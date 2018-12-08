#include "mp3.h"
#include "ID3v2.h"

#include "Song.h"

#include <exception>
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

	// This is a foreach loop (much more modern than for loops)
	for (auto &path : music_directory) {
		if (filesystem::is_directory(path)) {
			cout << "mp3_main: found a directory -- skipping (but will check its contents)" << endl;
			continue;
		}
		else if (filesystem::is_regular_file(path)) {
			cout << "mp3_main: found a regular file -- let's see if we can find mp3 information in it" << endl;
			
			
			try {
				Song song(path);

				// The runtime_error thrown in the constructor above will prevent this line from executing
				song.print();
			}
			catch (runtime_error &e) {
				// Just move on
			}
		}
	}
}