#include "Album.h"


Album::~Album()
{
}


Album::Album(string name) {
	this->name = name;
}


void Album::inherit_metadata() {
	for (auto &song : songs) {
		// Update list of genres
		if (count(genres.begin(), genres.end(), song->genre) == 0) {
			genres.push_back(song->genre);
		}

		// Update minimum
		if (year_range.first > song->year) {
			year_range.first = song->year;
		}

		// Update maximum
		if (year_range.second < song->year) {
			year_range.second = song->year;
		}

		// Copy the artist from the song (they should all be in agreement)
		artist = song->album_artist;
	}
}


void Album::reorder_songs() {
	map<int, Song*> songs_by_track_number;

	// Rely on the ordered nature of map to do the sorting for us
	for (auto &song : songs) {
		songs_by_track_number.insert({ song->track_of_album, song });
	}

	// Clear the list of songs this album has so we can fill it back up
	// with proper ordering
	songs.clear();
	
	// This will iterate in order for us
	for (auto &track_number_and_song : songs_by_track_number) {
		songs.push_back(track_number_and_song.second);
	}
}


void Album::print() {
	// Separator
	for (int i = 0; i < 60; i++) cout << "-";
	cout << endl;

	// Name
	cout << name << endl;
	
	// Separator
	for (int i = 0; i < 40; i++) cout << "-";
	cout << endl;

	// Year range
	cout << " " << year_range.first;
	if (year_range.first != year_range.second) cout << "-" << year_range.second;
	cout << endl;

	// Genres
	for (int i = 0, n = genres.size(); i < n; i++) {
		if (i != 0) cout << ", ";
		else cout << " ";
		
		cout << genres[i];
	}
	cout << endl;

	// Separator
	for (int i = 0; i < 40; i++) cout << "-";
	cout << endl;

	// Track listing
	for (int i = 0, n = songs.size(); i < n; i++) {
		cout << " " << i + 1 << ". " << songs[i]->title << endl;
	}

	// Separator
	for (int i = 0; i < 60; i++) cout << "-";
	cout << endl;
}