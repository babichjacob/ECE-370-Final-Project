#include "Song.h"


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
	int first_printable_index = -1;

	// Hunt down the first index of a "printable" character (because everything before it will be garbage)
	for (int i = 0; i < data_length; i++) {
		if (isprint(data[i])) {
			first_printable_index = i;
			break;
		}
	}

	// Then add all the characters after it to the string
	// (it occurred to me to use a substr method but I don't think that'll work because this isn't really a string or char array)
	for (int i = first_printable_index; i < data_length; i++) {
		if ((data[i] != 0))	data_str += data[i];
	}

	return data_str;
}


fs::path find_cache_file_path(fs::path music_file_path) {
	// Make a copy so we don't mutate the original path
	fs::path music_root_path = fs::path(music_file_path);
	fs::path cache_file_path = fs::path(CACHE_DIR);

	// Safety check: make sure we haven't called parent_path() until there are no parent paths left
	// (this could happen)
	while ((music_root_path.parent_path() != fs::path("bin/data/music")) && (music_root_path != fs::path(""))) {
		music_root_path = music_root_path.parent_path();
		cache_file_path /= music_root_path.filename();
	}

	return cache_file_path / music_file_path.filename().replace_extension("csv");
}


void Song::save_to_cache() {
	cache_file_path = find_cache_file_path(music_file_path);

	fs::create_directories(cache_file_path.parent_path());
	ofstream cache_file(cache_file_path.string());

	cache_file << title << ",";
	cache_file << album << ",";
	cache_file << artist << ",";
	cache_file << genre << ",";
	cache_file << track_of_album << ",";
	cache_file << year << endl;
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

	vector<string> parsed_csv = parse_csv_string_to_vector(csv_cache_line);

	title = parsed_csv[0];
	album = parsed_csv[1];
	artist = parsed_csv[2];
	genre = parsed_csv[3];
	track_of_album = parsed_csv[4];
	year = stoi(parsed_csv[5]);
}


void Song::set_from_file(fs::path path) {
	cout << "Song::set_from_file: about to set from file " << path.string() << endl;

	ID3v2::Tag mp3_tag(path.string());

	if (!mp3_tag.IsValid()) {
		cout << "Song::set_from_file: this is not a valid MP3 file -- bye" << endl;
		throw runtime_error("invalid file -- not MP3");
	}

	// Extract all the useful data (these can't all be turned into their own methods
	// because that was giving me a null pointer exception)
	ID3v2::Frame::v23::TIT2 *tit2 = dynamic_cast<ID3v2::Frame::v23::TIT2 *>(mp3_tag.GetFrameWithName("TIT2"));
	if (tit2 != nullptr) title = get_frame_data_as_string(tit2->GetData(), tit2->GetSize());
	else title = "Unknown Song";

	ID3v2::Frame::v23::TALB *talb = dynamic_cast<ID3v2::Frame::v23::TALB *>(mp3_tag.GetFrameWithName("TALB"));
	if (talb != nullptr) album = get_frame_data_as_string(talb->GetData(), talb->GetSize());
	else album = "Unknown Album";

	ID3v2::Frame::v23::TPE1 *tpe1 = dynamic_cast<ID3v2::Frame::v23::TPE1 *>(mp3_tag.GetFrameWithName("TPE1"));
	if (tpe1 != nullptr) artist = get_frame_data_as_string(tpe1->GetData(), tpe1->GetSize());
	else artist = "Unknown Artist";

	ID3v2::Frame::v23::TCON *tcon = dynamic_cast<ID3v2::Frame::v23::TCON *>(mp3_tag.GetFrameWithName("TCON"));
	if (tcon != nullptr) genre = get_frame_data_as_string(tcon->GetData(), tcon->GetSize());
	else genre = "Unknown Genre";

	ID3v2::Frame::v23::TRCK *trck = dynamic_cast<ID3v2::Frame::v23::TRCK *>(mp3_tag.GetFrameWithName("TRCK"));
	if (trck != nullptr) track_of_album = get_frame_data_as_string(trck->GetData(), trck->GetSize());
	else track_of_album = "1/1";

	ID3v2::Frame::v23::TYER *tyer = dynamic_cast<ID3v2::Frame::v23::TYER *>(mp3_tag.GetFrameWithName("TYER"));
	if (tpe1 != nullptr) year = stoi(get_frame_data_as_string(tyer->GetData(), tyer->GetSize()));
	else year = 1900;
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