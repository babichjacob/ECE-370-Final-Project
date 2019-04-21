#include "Song.h"

#include <fstream>
#include <string>


using std::cout;
using std::endl;
using std::ifstream;
using std::invalid_argument;
using std::ofstream;
using std::runtime_error;
using std::to_string;


Song::Song() {
}


Song::~Song()
{
}


Song::Song(fs::path path) {
	music_file_path = path;

	try {
		// Try to save computational power by loading information from the cache
		set_from_cache(path);
	}
	// But if that's not possible,
	catch (runtime_error &e) {
		if (string(e.what()).compare("not in cache") == 0) {
			// Parse the MP3 itself (slow and computationally expensive)
			set_from_file(path);
			// And save the results to the cache
			save_to_cache();
		}
	}
}


string get_frame_data_as_string(const uint8_t *data, int data_length) {
	string data_str = "";

	// Hunt down the printable characters
	// This is because the ID3v2 spec uses something weird call a "synch-safe integer"
	// so there are usually 0s between the values we need (and a few bytes of garbage before that)
	for (int i = 0; i < data_length; i++) {
		if (isprint(data[i])) {
			data_str += data[i];
		}
	}

	return data_str;
}


fs::path find_cache_file_path(fs::path music_file_path) {
	// Make a copy so we don't mutate the original path
	fs::path music_root_path = fs::path(music_file_path);
	fs::path cache_file_path = fs::path(CACHE_DIR);

	vector<fs::path> parents;

	// Safety check: make sure we haven't called parent_path() until there are no parent paths left
	// (this could happen)
	while ((music_root_path.parent_path() != fs::path(MUSIC_DIR)) && (music_root_path != fs::path(""))) {
		music_root_path = music_root_path.parent_path();
		parents.push_back(music_root_path.filename());
	}

	while (parents.size()) {
		// The / operator adds on a subfolder to the path
		cache_file_path /= parents.back();
		// For some reason, pop_back doesn't return the value it pops...
		parents.pop_back();
	}

	return cache_file_path / music_file_path.filename().replace_extension("csv");
}


void Song::save_to_cache() {
	cache_file_path = find_cache_file_path(music_file_path);

	fs::create_directories(cache_file_path.parent_path());
	ofstream cache_file(cache_file_path.string());

	// Write out all song data to the file
	cache_file << csv_encode({ title, album, artist, genre, to_string(track_of_album), to_string(year), album_artist, artwork_file_path.string(), is_favorited ? "1" : "0", to_string(plays) });
}


void Song::set_from_cache(fs::path path) {
	cache_file_path = find_cache_file_path(path);
	
	string csv_cache_line;

	// There's a race condition here but I'd rather ignore the possibility of that
	// than do EAFP in C++
	if (fs::exists(cache_file_path)) {
		ifstream cache_file(cache_file_path.string());
		
		// Read from the cache file
		getline(cache_file, csv_cache_line);
	}
	else {
		throw runtime_error("not in cache");
	}

	vector<string> parsed_csv = csv_decode(csv_cache_line);

	title = parsed_csv[0];
	album = parsed_csv[1];
	artist = parsed_csv[2];
	genre = parsed_csv[3];
	track_of_album = stoi(parsed_csv[4]);
	year = stoi(parsed_csv[5]);
	// Out of order because this was added after the fact
	album_artist = parsed_csv[6];
	artwork_file_path = fs::path(parsed_csv[7]);
	is_favorited = parsed_csv[8] == "1";
	plays = stoi(parsed_csv[9]);
}


void Song::set_from_file(fs::path path) {
	ID3v2::Tag mp3_tag(path.string());

	if (!mp3_tag.IsValid()) {
		throw runtime_error("invalid file -- not MP3");
	}

	// Extract all the useful data (these can't all be turned into their own methods
	// because that was giving me a null pointer exception)
	
	// Song title
	ID3v2::Frame::v23::TIT2 *tit2 = dynamic_cast<ID3v2::Frame::v23::TIT2 *>(mp3_tag.GetFrameWithName("TIT2"));
	if (tit2 != nullptr) title = get_frame_data_as_string(tit2->GetData(), tit2->GetSize());
	else title = "Unknown Song";

	// Album
	ID3v2::Frame::v23::TALB *talb = dynamic_cast<ID3v2::Frame::v23::TALB *>(mp3_tag.GetFrameWithName("TALB"));
	if (talb != nullptr) album = get_frame_data_as_string(talb->GetData(), talb->GetSize());
	else album = "Unknown Album";

	// Track / song artist
	ID3v2::Frame::v23::TPE1 *tpe1 = dynamic_cast<ID3v2::Frame::v23::TPE1 *>(mp3_tag.GetFrameWithName("TPE1"));
	if (tpe1 != nullptr) artist = get_frame_data_as_string(tpe1->GetData(), tpe1->GetSize());
	else artist = "Unknown Artist";

	// Album artist
	ID3v2::Frame::v23::TPE2 *tpe2 = dynamic_cast<ID3v2::Frame::v23::TPE2 *>(mp3_tag.GetFrameWithName("TPE2"));
	if (tpe2 != nullptr) album_artist = get_frame_data_as_string(tpe2->GetData(), tpe2->GetSize());
	else album_artist = "Unknown Artist";

	// Genre
	ID3v2::Frame::v23::TCON *tcon = dynamic_cast<ID3v2::Frame::v23::TCON *>(mp3_tag.GetFrameWithName("TCON"));
	if (tcon != nullptr) genre = get_frame_data_as_string(tcon->GetData(), tcon->GetSize());
	else genre = "Unknown Genre";

	// Track number of album
	ID3v2::Frame::v23::TRCK *trck = dynamic_cast<ID3v2::Frame::v23::TRCK *>(mp3_tag.GetFrameWithName("TRCK"));
	// It often happens that this field is something like "4/7", or it could just be "3" (for example)
	// in other cases (both are valid) but we just want that 4 or that 3 and disregard the 7.
	// Thankfully, `stoi` does that for us
	// Sadly, it can raise an `invalid_argument` error and we need to catch that and use the default value (1) instead
	try {
		if (trck != nullptr) track_of_album = stoi(get_frame_data_as_string(trck->GetData(), trck->GetSize()));
		else track_of_album = 1;
	}
	catch (invalid_argument &e) {
		// (void)ing a variable means to ignore it (the compiler gives a warning for not using this variable)
		(void)e;
		track_of_album = 1;
	}

	// Year
	ID3v2::Frame::v23::TYER *tyer = dynamic_cast<ID3v2::Frame::v23::TYER *>(mp3_tag.GetFrameWithName("TYER"));
	// See above for handling the cases where the year is missing from the data
	try {
		if (tpe1 != nullptr) year = stoi(get_frame_data_as_string(tyer->GetData(), tyer->GetSize()));
		else year = 1900;
	}
	catch (invalid_argument &e) {
		(void)e;
		year = 1900;
	}

	// Cover artwork
	ID3v2::Frame::v23::APIC *apic = dynamic_cast<ID3v2::Frame::v23::APIC *>(mp3_tag.GetFrameWithName("APIC"));
	// Mimetype (whether it is jpg, png, ...)
	string mimetype = apic->GetMimeType();

	// Make a copy of the `music_file_path` so we don't mutate the original
	artwork_file_path = find_cache_file_path(fs::path(music_file_path).replace_filename("artwork"));

	// Determine the correct extension to match with the mimetype
	if      (mimetype.compare("image/jpeg") == 0) artwork_file_path.replace_extension("jpeg");
	else if (mimetype.compare("image/png")  == 0) artwork_file_path.replace_extension("png");
	else                                          artwork_file_path.replace_extension("unknown");

	// Only save the image if the file doesn't already exist
	if (!fs::exists(artwork_file_path)) {
		cout << "about to write out artwork file" << endl;

		// Create the folder so that ofstream actually works
		fs::create_directories(artwork_file_path.parent_path());

		// Write out raw binary
		ofstream artwork_file(artwork_file_path.string(), ofstream::binary);

		vector<uint8_t> all_binary_data = apic->GetPictureData();

		cout << "about to write out " << sizeof(uint8_t)*all_binary_data.size() << " bytes" << endl;

		for (auto &binary_data : all_binary_data) {
			artwork_file.write((char *) &binary_data, sizeof(binary_data));
		}

		artwork_file.close();
		cout << "successfully wrote out artwork file " << artwork_file_path.string() << endl << endl;
	}
	else cout << "didn't need to write out artwork file -- it's already there " << artwork_file_path.string() << endl << endl;

	// Is favorited (false because this song must be new to the user's library)
	is_favorited = false;

	// Play count (0 because this song must be new to the user's library)
	plays = 0;
}

void Song::print() {
	for (int i = 0; i < 40; i++) cout << "-";
	cout << endl;

	cout << title << " (" << track_of_album << "): " << endl;
	cout << "\t Artist: " << artist << endl;
	cout << "\t Album: "  << album << endl;
	cout << "\t Genre: "  << genre << endl;
	cout << "\t Year: "   << year << endl;

	for (int i = 0; i < 40; i++) cout << "-";
	cout << endl;
}

void Song::increment_plays() {
	plays++;
	// Update the cache
	save_to_cache();
}

void Song::toggle_favorited() {
	is_favorited = !is_favorited;
	save_to_cache();
}