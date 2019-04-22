#include "library.h"

#include <algorithm>
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
using std::stable_sort;
using std::unordered_map;


bool find_all_songs_incrementally(Songs* previous_effort, fs::recursive_directory_iterator* music_directory, int* index) {
	// This is a foreach loop (much more modern than for loops)

	fs::recursive_directory_iterator end_iterator;

	while (true) {
		(*music_directory)++;

		// Quit when the iterator is exhausted
		if (*music_directory == end_iterator) return false;

		// Get the path the iterator currently points to
		fs::directory_entry path = **music_directory;

		// Skip directories
		if (fs::is_directory(path)) continue;

		// Check files
		if (fs::is_regular_file(path)) {
			try {
				Song* song = new Song(path.path());

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


Albums build_albums(Songs songs) {
	Albums albums_map;

	for (auto& song : songs) {
		pair<string, string> key(song->album_artist, song->album);
		// Find a pre-existing album
		if (albums_map.count(key) > 0) {
			Albums::iterator map_entry = albums_map.find(key);
			Album* this_album = &map_entry->second;
			// Add this song to the album
			this_album->songs.push_back(song);
		}
		// Or make one if necessary
		else {
			Album new_album(song->album);
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


void sort_songs(Songs* songs, vector<string> by) {
	// Iterate in reverse over the sorting keys
	for (auto it = by.rbegin(); it != by.rend(); it++) {
		string key = *it;

		auto comparator = [&key](const auto & left, const auto & right) {
			if (key == "album") {
				return left->album < right->album;
			}
			else if (key == "album_artist") {
				return left->album_artist < right->album_artist;
			}
			else if (key == "artist") {
				return left->artist < right->artist;
			}
			else if (key == "genre") {
				return left->genre < right->genre;
			}
			else if (key == "is_favorited") {
				// Descending too (i.e. favorited before unfavorited)
				return left->is_favorited > right->is_favorited;
			}
			else if (key == "plays") {
				// Prefer descending order (hence > instead of <) for play count
				return left->plays > right->plays;
			}
			else if (key == "title") {
				return left->title < right->title;
			}
			else if (key == "track_of_album") {
				return left->track_of_album < right->track_of_album;
			}
			else if (key == "year") {
				// Descending
				return left->year > right->year;
			}
			// Do nothing because this key is not recognized
			return false;
		};

		cout << "sorting songs by " << key << endl;
		stable_sort(songs->begin(), songs->end(), comparator);
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