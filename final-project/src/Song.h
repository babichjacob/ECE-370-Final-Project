#pragma once

#include "ID3v2.h"
#include <filesystem>
using namespace std;
// This is a new feature I need
namespace filesystem = std::experimental::filesystem;

class Song
{
private:
	string get_frame_data_as_string(const uint8_t *data, int data_length);

public:
	bool is_valid = false;

	string title;
	string album;
	string artist;
	int year;

	Song();
	Song(filesystem::directory_entry path);
	~Song();
	void set_from_file(filesystem::directory_entry path);
	void print();
};

