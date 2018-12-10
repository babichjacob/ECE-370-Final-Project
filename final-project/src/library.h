#pragma once

#include <filesystem>
#include <iostream>
#include <string>
#include <unordered_map>

#include "Album.h"
#include "Artist.h"
#include "Song.h"

using std::cout;
using std::endl;
using std::unordered_map;

namespace fs = std::experimental::filesystem;

vector<Song> find_all_songs();
unordered_map<string, Album> build_albums(vector<Song> songs);
unordered_map<string, Artist> build_artists(unordered_map<string, Album> albums);
void mp3_main();