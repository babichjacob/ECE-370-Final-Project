#pragma once

#include <filesystem>
#include <iostream>
#include <map>
#include <string>
#include <unordered_map>

#include "Album.h"
#include "Artist.h"
#include "Song.h"

using std::cout;
using std::endl;
using std::map;
using std::unordered_map;

namespace fs = std::experimental::filesystem;

vector<Song> find_all_songs();
map<string, Album> build_albums(vector<Song> songs);
map<string, Artist> build_artists(map<string, Album> albums);
void mp3_main();