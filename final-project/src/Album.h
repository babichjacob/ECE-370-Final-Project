#pragma once

#include <map>
#include <string>

#include "Song.h"

using std::count;
using std::cout;
using std::endl;
using std::map;
using std::pair;

class Album
{
public:
	string name;
	Songs songs;

	// Inherited metadata (from the songs in the album)
	string artist;
	vector<string> genres;
	// Give the lower bound an absurdly high value to guarantee the year of any song will be able to
	// overcome it, and the higher bound an absurdly low value for the same reason
	pair<int, int> year_range = { 999999, 0 };

	~Album();
	Album(string name);
	void inherit_metadata();
	void reorder_songs();
	void print();
};


typedef map<string, Album> Albums;