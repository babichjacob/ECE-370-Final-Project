#include "mp3.h"
#include "ID3v2.h"

#include <filesystem>
#include <fstream>
#include <iostream>
using namespace std;
// This is a new feature I need
namespace filesystem = std::experimental::filesystem;


int chars_to_int(char char_in[]) {
	return (char_in[0] << 24) + (char_in[1] << 16) + (char_in[2] << 8) + (char_in[3] << 0);
}


void mp3_main() {
	cout << "mp3_main: Hello!" << endl;

	string music_path = "bin/data/Music";
	filesystem::recursive_directory_iterator music_directory = filesystem::recursive_directory_iterator(music_path);
	ID3v2::AbstractFrame                          *frame;

	// This is a foreach loop (much more modern than for loops)
	for (auto &path : music_directory) {
		cout << "mp3_main: Found a file: " << path.path() << endl;
		ID3v2::Tag mp3_tag(path.path().string());
		ID3v2::AbstractFrame *frame;
		vector<ID3v2::AbstractFrame *> frames;
		vector< ID3v2::AbstractFrame * >::iterator iter;

		if (mp3_tag.IsValid()) {
			cout << "ID3v2 version: " << mp3_tag.GetVersion().GetStringValue() << endl;
			frames = mp3_tag.GetFrames();

			ID3v2::Frame::v23::TIT2 *tit2;
			tit2 = dynamic_cast<ID3v2::Frame::v23::TIT2 *>(mp3_tag.GetFrameWithName("TIT2"));
			cout << (tit2 == nullptr ? "Null pointer" : "Not null pointer") << endl;
			const uint8_t *data = tit2->GetData();
			int data_length = tit2->GetSize();
			string data_str = "";
			for (int i = 3; i < data_length; i++) {
				if ((data[i] != 0))	data_str += data[i];
			}
			cout << "Title: " << data_str << endl;
		}
	}
}