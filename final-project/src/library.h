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


Songs find_all_songs();
Albums build_albums(Songs songs);
Artists build_artists(Albums albums);

Songs rebuild_songs(Artists artists_map, int number_of_songs);

void mp3_main();