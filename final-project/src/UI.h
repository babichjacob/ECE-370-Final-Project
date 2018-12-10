#pragma once

#include "ofMain.h"

using std::max;

class UI
{
public:
	int frame_loaded = -1;

	UI();
	~UI();
	void setup();
	void update();
	void draw();

private:
	ofTrueTypeFont font_small;
	int font_small_size = 9;

	ofTrueTypeFont font_medium;
	int font_medium_size = 13;

	ofTrueTypeFont font_large;
	int font_large_size = 18;

	ofRectangle menu_bar;

	int num_menu_bar_top_level_entries = 4;
	string menu_bar_top_level_entries[4] = { "File", "Edit", "View", "About" };
};

