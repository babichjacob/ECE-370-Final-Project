#include "mp3.h"
#include "ID3v2.h"

#include <filesystem>
#include <fstream>
#include <iostream>

using namespace std;
// This is a new feature I need
namespace filesystem = std::experimental::filesystem;


void mp3_main() {
	cout << "mp3_main: Hello!" << endl;

	string music_path = "bin/data/Music";
	filesystem::recursive_directory_iterator music_directory = filesystem::recursive_directory_iterator(music_path);
	ID3v2::AbstractFrame *frame;

	// This is a foreach loop (much more modern than for loops)
	for (auto &path : music_directory) {
		if (filesystem::is_directory(path)) {
			cout << "mp3_main: found a directory -- skipping (but will check its contents)" << endl;
			continue;
		}
		else if (filesystem::is_regular_file(path)) {
			cout << "mp3_main: found a regular file -- let's see if we can find mp3 information in it";
		}
		
		ID3v2::Tag mp3_tag(path.path().string());
		ID3v2::AbstractFrame *frame;
		vector<ID3v2::AbstractFrame *> frames;
		vector< ID3v2::AbstractFrame * >::iterator iter;

		if (mp3_tag.IsValid()) {
			cout << "ID3v2 version: " << mp3_tag.GetVersion().GetStringValue() << endl;
			frames = mp3_tag.GetFrames();

			ID3v2::Frame::v23::TIT2 *tit2 = dynamic_cast<ID3v2::Frame::v23::TIT2 *>(mp3_tag.GetFrameWithName("TIT2"));
			
			const uint8_t *data = tit2->GetData();
			string data_str = "";
			int data_length = tit2->GetSize();

			int first_alphanumerical_index = -1;

			for (int i = 0; i < data_length; i++) {
				if (isalnum(data[i])) {
					first_alphanumerical_index = i;
					break;
				}
			}

			for (int i = first_alphanumerical_index; i < data_length; i++) {
				if ((data[i] != 0))	data_str += data[i];
			}
			cout << "Title: " << data_str << endl;
		}
	}
}