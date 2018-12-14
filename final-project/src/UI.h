#pragma once

#include <filesystem>

#include "ofMain.h"
#include "Song.h"

constexpr auto ICONS_DIR = "bin/data/.MyTunes/icons";

namespace fs = std::experimental::filesystem;

using std::map;
using std::max;
using std::min;


// An icon set is an ofImage and other stuff relating to it
struct IconBundle {
	ofImage icon;
	ofRectangle hitbox;
	ofColor color_inactive;
	ofColor color_active;
};

typedef struct IconBundle IconBundle;


class UI
{
public:
	int frame_loaded = -1;

	// How long the fade-in animation will last
	int transition_duration_frames = 60;

	UI();
	~UI();
	void setup();
	void update();
	void draw();

	void draw_artist_view();
	void draw_album_view();
	void draw_song_view(vector<Song> all_songs);

private:
	// Color palette
	ofColor cool_gray_lightest;
	ofColor cool_gray_lighter;
	ofColor cool_gray_light;
	ofColor cool_gray;
	ofColor cool_gray_darker;
	ofColor cool_gray_darkest;
	ofColor cool_black;
	// End color palette

	// Fonts
	ofTrueTypeFont font_small;
	int font_small_size = 9;

	ofTrueTypeFont font_medium;
	int font_medium_size = 13;

	ofTrueTypeFont font_large;
	int font_large_size = 18;
	// End fonts

	// The standard amount of padding inside / around elements
	int padding_standard = 16;

	ofRectangle play_zone;
	ofRectangle currently_playing_zone;

	// Icons
	map<string, IconBundle> icons;


	ofImage currently_playing_song_image;

	ofRectangle columns;
	vector<string> columns_entries = {"Song Name", "Album", "Artist", "Genre", "Year"};
	vector<int> columns_edges = {0, 500, 800, 1100, 1400};
};


string get_icon_path(string icon_name);
int get_icon_baseline(ofRectangle play_zone, ofImage icon);