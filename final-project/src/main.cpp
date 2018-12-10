#include <exception>
#include <filesystem>
#include <fstream>
#include <iostream>

using std::cout;
using std::endl;

namespace fs = std::experimental::filesystem;

#include "Album.h"
#include "ofMain.h"
#include "ofApp.h"
#include "ID3v2.h"
#include "Song.h"


void mp3_main() {
	cout << "mp3_main: Hello!" << endl;

	fs::recursive_directory_iterator music_directory = fs::recursive_directory_iterator(MUSIC_DIR);

	unordered_map<string, Album> albums_map;


	// This is a foreach loop (much more modern than for loops)
	for (auto &path : music_directory) {
		if (fs::is_directory(path)) {
			cout << "mp3_main: found a directory -- skipping (but will check its contents)" << endl;
			continue;
		}
		else if (fs::is_regular_file(path)) {
			cout << "mp3_main: found a regular file -- let's see if we can find mp3 information in it" << endl;


			try {
				Song song(path.path());

				// The runtime_error thrown in the constructor above will prevent every line from here on from executing
				// song.print();

				// Pretend this declaration statement isn't here (it interferes with the explanations below)
				Album *this_album;

				// Find a pre-existing album
				if (albums_map.count(song.album) > 0) {
					unordered_map<string, Album>::iterator map_entry = albums_map.find(song.album);
					// Add this song to the album
					this_album = &map_entry->second;
				}
				// Or make one if necessary
				else {
					Album new_album(song.album);
					this_album = &new_album;
					// Add this song to the album
					pair<string, Album> map_entry(song.album, new_album);
					albums_map.insert(map_entry);
				}
				
				this_album->songs.push_back(song);
			}
			catch (runtime_error &e) {
				// Just move on
				(void)e;
			}
		}
	}

	// Let's examine our library now that the directories have been completely iterated over
	cout << endl << endl;
	for (auto &map_entry : albums_map) {
		cout << map_entry.first << " [";
		for (int i = 0, n = map_entry.second.songs.size(); i < n; i++) {
			cout << map_entry.second.songs[i].title << " ";
		}
		cout << "]" << endl << endl;
	}
}

//========================================================================
int main( ){
	cout << "main: begin program" << endl;

	cout << "main: testing - switching to mp3_main now" << endl;

	mp3_main();

	cout << "main: testing - mp3_main exited and the program will too now" << endl;
	return 0;

	// Default, dummy size. Replaced two lines below.
	ofSetupOpenGL(1280,720,OF_WINDOW);			// <-------- setup the GL context

	// Top left
	ofSetWindowPosition(0, 0);

	// Full size (not to be confused with full-screen or maximized because it is neither of these)
	ofSetWindowShape(ofGetScreenWidth(), ofGetScreenHeight());

	// this kicks off the running of my app
	// can be OF_WINDOW or OF_FULLSCREEN
	// pass in width and height too:
	cout << "main: running openFrameworks app" << endl;
	ofRunApp(new ofApp());
}
