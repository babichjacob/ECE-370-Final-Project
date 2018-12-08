#include <exception>
#include <filesystem>
#include <fstream>
#include <iostream>


#include "ID3v2.h"
#include "Song.h"

#include "mp3.h"


void mp3_main() {
	cout << "mp3_main: Hello!" << endl;

	filesystem::recursive_directory_iterator music_directory = filesystem::recursive_directory_iterator(MUSIC_DIR);

	// This is a foreach loop (much more modern than for loops)
	for (auto &path : music_directory) {
		if (filesystem::is_directory(path)) {
			cout << "mp3_main: found a directory -- skipping (but will check its contents)" << endl;
			continue;
		}
		else if (filesystem::is_regular_file(path)) {
			cout << "mp3_main: found a regular file -- let's see if we can find mp3 information in it" << endl;
			
			
			try {
				Song song(path.path());

				// The runtime_error thrown in the constructor above will prevent this line from executing
				song.print();
			}
			catch (runtime_error &e) {
				// Just move on
				(void) e;
			}
		}
	}
}