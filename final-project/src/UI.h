#include "ofMain.h"

#pragma once

class UI
{
public:
	UI();
	~UI();
	void setup();
	void update();
	void draw();

private:
	ofRectangle menu_bar;

	int num_menu_bar_top_level_entries;
	string menu_bar_top_level_entries[4];
};

