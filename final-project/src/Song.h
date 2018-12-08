#pragma once

#include "ID3v2.h"
#include <filesystem>
using namespace std;
// This is a new feature I need
namespace filesystem = std::experimental::filesystem;

class Song
{
private:
	void set_from_file(filesystem::directory_entry path);

public:
	// Flag variable -- there's no way to know whether a given file will be valid
	// until you try to instantiate a Song object from it
	bool is_valid = false;

	string title;
	string album;
	string artist;
	string genre;
	string track_of_album;
	int year;

	Song();
	Song(filesystem::directory_entry path);
	~Song();
	void print();
};

