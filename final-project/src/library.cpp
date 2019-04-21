#include "library.h"

#include <filesystem>
#include <iostream>
#include <iterator>
#include <map>
#include <string>
#include <unordered_map>

using std::advance;
using std::cout;
using std::endl;
using std::map;
using std::runtime_error;
using std::unordered_map;


bool find_all_songs_incrementally(Songs* previous_effort, fs::recursive_directory_iterator* music_directory, int* index) {
	// This is a foreach loop (much more modern than for loops)

	fs::recursive_directory_iterator end_iterator;

	while (true) {
		(*music_directory)++;

		if (*music_directory == end_iterator) return false;

		fs::directory_entry path = **music_directory;

		if (fs::is_directory(path)) {
			cout << "mp3_main: found a directory -- skipping (but will check its contents)" << endl;

			continue;
		}

		if (fs::is_regular_file(path)) {
			cout << "mp3_main: found a regular file -- let's see if we can find mp3 information in it" << endl;

			try {
				Song song(path.path());

				cout << "loaded" << endl;
				song.print();

				// The runtime_error thrown in the constructor above will prevent every line from here on from executing
				// if Song object creation failed
				// in other words, this code will only execute for valid songs (MP3 files)
				previous_effort->push_back(song);

				// Signal that it's possible for there to be more songs
				return true;
			}
			catch (runtime_error & e) {
				// Just move on; it's not a song
				(void)e;
			}
		}
	}
}


//Songs find_all_songs() {
//	fs::recursive_directory_iterator music_directory = fs::recursive_directory_iterator(MUSIC_DIR);
//	Songs all_songs;
//	
//	// This is a foreach loop (much more modern than for loops)
//	for (auto &path : music_directory) {
//		if (fs::is_directory(path)) {
//			cout << "mp3_main: found a directory -- skipping (but will check its contents)" << endl;
//			continue;
//		}
//		else if (fs::is_regular_file(path)) {
//			cout << "mp3_main: found a regular file -- let's see if we can find mp3 information in it" << endl;
//
//			try {
//				Song song(path.path());
//
//				// The runtime_error thrown in the constructor above will prevent every line from here on from executing
//				// if Song object creation failed
//				// in other words, this code will only execute for valid songs (MP3 files)
//				all_songs.push_back(song);
//			}
//			catch (runtime_error &e) {
//				// Just move on
//				(void)e;
//			}
//		}
//	}
//
//	return all_songs;
//}


Albums build_albums(Songs songs) {
	Albums albums_map;

	for (auto& song : songs) {
		pair<string, string> key(song.album_artist, song.album);
		// Find a pre-existing album
		if (albums_map.count(key) > 0) {
			Albums::iterator map_entry = albums_map.find(key);
			Album* this_album = &map_entry->second;
			// Add this song to the album
			this_album->songs.push_back(song);
		}
		// Or make one if necessary
		else {
			Album new_album(song.album);
			// Add this song to the album (has to be done before adding it to the map)
			new_album.songs.push_back(song);
			pair<pair<string, string>, Album> map_entry(key, new_album);
			albums_map.insert(map_entry);
		}
	}

	return albums_map;
}

Artists build_artists(Albums albums) {
	Artists artists_map;

	for (auto& albums_map_entry : albums) {
		// Re-order the songs so that they are in the order of track number
		albums_map_entry.second.reorder_songs();

		// Load the metadata (genre and year for now) from the album's songs into the album itself
		albums_map_entry.second.inherit_metadata();

		string artist_name = albums_map_entry.second.artist;

		// Find a pre-existing artist
		if (artists_map.count(artist_name) > 0) {
			Artists::iterator artists_map_entry = artists_map.find(artist_name);
			Artist* this_artist = &artists_map_entry->second;
			// Add this album to the artist
			this_artist->albums.push_back(albums_map_entry.second);
		}
		// Or make one if necessary
		else {
			Artist new_artist(artist_name);
			// Add this album to the artist (has to be done before adding it to the map)
			new_artist.albums.push_back(albums_map_entry.second);
			pair<string, Artist> artists_map_entry(artist_name, new_artist);
			artists_map.insert(artists_map_entry);
		}
	}

	return artists_map;
}


Songs rebuild_songs(Artists artists_map, int number_of_songs) {
	// Create a new vector of songs sorted by artist, then by album
	// (though this should be the case by default for a well-maintained library,
	//  not every library is well-maintained)
	Songs sorted_songs;

	// Open up enough space for the number of songs we already have
	sorted_songs.reserve(number_of_songs);

	// For every artist,
	for (auto& artists_map_entry : artists_map) {
		// For each of their albums,
		// (we assume that `reorder_songs()` has already been called for each album)
		for (auto& album : artists_map_entry.second.albums) {
			// Do something called "extending" the vector
			// AKA adding all the contents from one vector to another
			sorted_songs.insert(sorted_songs.end(), album.songs.begin(), album.songs.end());
		}
	}

	return sorted_songs;
}

void sort_songs(Songs* songs, vector<string> by) {

	// Iterate in reverse over the sorting keys
	for (auto it = by.rbegin(); it != by.rend(); it++) {
		string key = *it;

		auto comparator = [&key](const auto & left, const auto & right) {
			if (key == "album_artist") {
				return left.album_artist < right.album_artist;
			}
			else if (key == "plays") {
				return left.album_artist < right.album_artist;
			}
			else {
				// Do nothing because this key is not recognized
				return true;
			}
		};

		cout << "sorting songs by " << key << endl;
		sort(songs->begin(), songs->end(), comparator);
	}
}


// deprecation notice -- please use openFrameworks for this (only here in case it becomes necessary to use again)
void mp3_main() {
	cout << "mp3_main: Hello!" << endl;

	Songs all_songs;
	int loaded_index = 0;

	fs::recursive_directory_iterator music_directory(MUSIC_DIR);
	while (find_all_songs_incrementally(&all_songs, &music_directory, &loaded_index));

	Albums albums_map = build_albums(all_songs);
	Artists artists_map = build_artists(albums_map);

	// Now examine our artists
	cout << endl;
	for (auto& artists_map_entry : artists_map) {
		for (int i = 0; i < 80; i++) cout << "-";
		cout << endl;

		cout << artists_map_entry.second.name << endl;
		for (auto& album : artists_map_entry.second.albums) {
			album.print();
		}

		for (int i = 0; i < 80; i++) cout << "-";
		cout << endl << endl;
	}
}