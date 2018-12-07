#include "mp3.h"

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

	// This is a foreach loop (much more modern than for loops)
	for (auto &path : music_directory) {
		cout << "mp3_main: Found a file: " << path.path() << endl;

		ifstream mp3_file(path.path(), ifstream::in | ifstream::binary);

		// Skip first 6 bytes (until we end up at byte 7)
		mp3_file.ignore(6);

		char header_length[4];
		mp3_file.read(header_length, 4);

		int header_length_as_int = chars_to_int(header_length);

		printf("%d\n", header_length_as_int);

		mp3_file.close();
	}
}