#pragma once

#include <map>
#include <string>
#include <vector>

#include "Album.h"


using std::map;
using std::string;
using std::vector;


class Artist
{
public:
	string name;
	vector<Album> albums;
	
	Artist(string name);
	~Artist();
};


typedef map<string, Artist> Artists;