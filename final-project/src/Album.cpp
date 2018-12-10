#include "Album.h"



Album::Album()
{
}


Album::~Album()
{
	cout << "a new album object was made without args" << endl;
}


Album::Album(string name) {
	this->name = name;

	// Give it an absurdly high value to guarantee the year of any song will be able to
	// overcome it
	year_range.first = 999999;
	// Same reason as before, but low value
	year_range.second = 0;

	cout << "a new album object was made with args" << endl;
}


void Album::inherit_metadata() {
	for (auto &song : songs) {
		// Update list of genres
		if (count(genres.begin(), genres.end(), song.genre) == 0) {
			genres.push_back(song.genre);
		}

		// Update minimum
		if (year_range.first > song.year) {
			year_range.first = song.year;
		}

		// Update maximum
		if (year_range.second < song.year) {
			year_range.second = song.year;
		}
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
	for (int i = 0; i < songs.size(); i++) {
		cout << " " << i + 1 << ". " << songs[i].title << endl;
	}

	// Separator
	for (int i = 0; i < 60; i++) cout << "-";
	cout << endl;
}