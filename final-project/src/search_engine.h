#pragma once

#include <map>
#include <string>
#include <vector>

#include "song.h"

using std::map;
using std::string;
using std::vector;


typedef map<string, vector<Song*>> SearchEngine;


void search_index(SearchEngine* se, Song* song);
vector<Song*> search_for(SearchEngine* se, string text);

// Part of the private API
void search_connect_string_to_song(SearchEngine* se, Song* song, string text);