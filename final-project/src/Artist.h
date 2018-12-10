#pragma once

#include "Album.h"

#include <string>

using std::string;


class Artist
{
public:
	string name;
	vector<Album> albums;
	
	Artist(string name);
	~Artist();
};

