#pragma once

#include "ofMain.h"
#include "Song.h"

using std::max;
using std::min;


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
	// Colors
	ofColor cool_gray_lightest;
	ofColor cool_gray_lighter;
	ofColor cool_gray_light;
	ofColor cool_gray;
	ofColor cool_gray_darker;
	ofColor cool_gray_darkest;
	ofColor cool_black;
	// End colors

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
	ofImage previous_song_icon;
	ofImage next_song_icon;

	ofImage rewind_icon;
	ofImage forward_icon;

	ofImage play_icon;
	ofImage pause_icon;
	// End icons

	ofImage currently_playing_song_image;

	ofRectangle columns;
	vector<string> columns_entries = {"Song Name", "Album", "Artist", "Genre", "Year"};
	vector<int> columns_edges = {0, 500, 800, 1100, 1400};
};


int get_icon_baseline(ofRectangle play_zone, ofImage icon);