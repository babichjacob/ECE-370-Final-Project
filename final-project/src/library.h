#pragma once

#include "Album.h"
#include "Artist.h"
#include "Song.h"

namespace fs = std::experimental::filesystem;


Songs find_all_songs();
Albums build_albums(Songs songs);
Artists build_artists(Albums albums);

Songs rebuild_songs(Artists artists_map, int number_of_songs);

void mp3_main();