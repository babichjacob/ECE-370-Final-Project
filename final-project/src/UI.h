#pragma once

#include "ofMain.h"

using std::max;


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

private:
	ofColor cool_gray_lightest;
	ofColor cool_gray_lighter;
	ofColor cool_gray_light;
	ofColor cool_gray;
	ofColor cool_gray_darker;

	ofTrueTypeFont font_small;
	int font_small_size = 9;

	ofTrueTypeFont font_medium;
	int font_medium_size = 13;

	ofTrueTypeFont font_large;
	int font_large_size = 18;

	ofRectangle play_zone;

	ofRectangle columns;
	vector<string> columns_entries = {"Song Name", "Album", "Artist", "Genre", "Year"};
	vector<int> columns_edges = {0, 500, 800, 1100, 1400};
};

