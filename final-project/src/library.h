#pragma once

#include <string>
#include <vector>

#include "Album.h"
#include "Artist.h"
#include "search_engine.h"
#include "Song.h"

using std::string;
using std::vector;

namespace fs = std::experimental::filesystem;

bool find_all_songs_incrementally(Songs* previous_effort, fs::recursive_directory_iterator* music_directory, int* index, SearchEngine* se);
Albums build_albums(Songs songs);
Artists build_artists(Albums albums);

void sort_songs(Songs* songs, vector<string> by = { "album_artist", "album", "track_of_album" });

void mp3_main();