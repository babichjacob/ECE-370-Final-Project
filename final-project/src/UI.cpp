#include "UI.h"
#include "ofMain.h"


UI::UI()
{
}


UI::~UI()
{
}

void UI::setup() {
	font_small.load("Heebo/Heebo-Regular.ttf", font_small_size);
	font_medium.load("Heebo/Heebo-Light.ttf", font_medium_size);
	font_large.load("Heebo/Heebo-Thin.ttf", font_large_size);


	menu_bar.x = 0;
	menu_bar.y = 0;



	num_menu_bar_top_level_entries = 4;

	menu_bar_top_level_entries[0] = "File";
	menu_bar_top_level_entries[1] = "Edit";
	menu_bar_top_level_entries[2] = "View";
	menu_bar_top_level_entries[3] = "About";
}

void UI::update() {
	menu_bar.width = ofGetWidth();
	menu_bar.height = 30;
}

void UI::draw() {
	ofSetColor(ofColor::silver);
	ofDrawRectangle(menu_bar);
}