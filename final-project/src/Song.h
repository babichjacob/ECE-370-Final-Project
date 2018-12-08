#pragma once

#include <chrono>
#include <filesystem>
#include <thread>

#include "csv.h"
#include "ID3v2.h"

constexpr auto MUSIC_DIR = "bin/data/music";
constexpr auto CACHE_DIR = "bin/data/.MyTunes/cache";

using namespace std;
// This is a new feature I need
namespace filesystem = std::experimental::filesystem;


class Song
{
private:
	void save_to_cache();
	void set_from_cache(filesystem::path path);
	void set_from_file(filesystem::path path);

public:
	string title;
	string album;
	string artist;
	string genre;
	// Ex 4/7
	string track_of_album;
	int year;

	filesystem::path file_path;

	Song();
	Song(filesystem::path path);
	~Song();
	void print();
};

