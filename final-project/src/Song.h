#pragma once

#include <filesystem>

#include "csv.h"
#include "ID3v2.h"

constexpr auto MUSIC_DIR = "bin/data/music";
constexpr auto CACHE_DIR = "bin/data/.MyTunes/cache";

namespace fs = std::experimental::filesystem;

class Song
{
private:
	fs::path cache_file_path;

	void save_to_cache();
	void set_from_cache(fs::path path);
	void set_from_file(fs::path path);

public:
	string title;
	string album;
	string artist;
	string genre;
	// Ex 4 (out of the total number of songs in the album)
	int track_of_album = 0;
	int year = 1900;
	// Out of order because these were added after the fact
	string album_artist;
	fs::path artwork_file_path;
	bool is_favorited = false;
	int plays = 0;


	fs::path music_file_path;


	Song();
	Song(fs::path path);
	~Song();
	void print();
	void increment_plays();
	void toggle_favorited();
};

typedef vector<Song*> Songs;