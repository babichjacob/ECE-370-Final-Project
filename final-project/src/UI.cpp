#include "UI.h"
#include "ofMain.h"


UI::UI()
{
}


UI::~UI()
{
}

void UI::setup() {
	cout << "UI::setup: " << "about to load fonts" << endl;
	font_small.load("Heebo/Heebo-Regular.ttf", font_small_size);
	font_medium.load("Heebo/Heebo-Light.ttf", font_medium_size);
	font_large.load("Heebo/Heebo-Thin.ttf", font_large_size);
	cout << "UI::setup: " << "fonts loaded" << endl;


	menu_bar.x = 0;
	menu_bar.y = 0;
}

void UI::update() {
	menu_bar.width = ofGetWidth();
	menu_bar.height = 30;
}

void UI::draw() {
	ofSetColor(ofColor::silver);
	ofDrawRectangle(menu_bar);
}