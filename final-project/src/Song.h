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
	int track_of_album;
	int year;
	// Out of order because these were added after the fact
	string album_artist;
	fs::path artwork_file_path;
	bool is_favorited;
	int plays;


	fs::path music_file_path;


	Song();
	Song(fs::path path);
	~Song();
	void print();
	void increment_plays();
};

typedef vector<Song> Songs;