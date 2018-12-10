#include "UI.h"


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
	// Don't draw a UI until the songs are loaded in
	if (frame_loaded == -1) return;

	// Dummy: pretend the UI is this blue rectangle
	ofSetColor(ofColor::blue);
	ofDrawRectangle(ofGetWidth() / 4, ofGetHeight() / 4, ofGetWidth() / 2, ofGetHeight() / 2);

	int transition_duration_frames = 90;
	
	if (ofGetFrameNum() - frame_loaded < transition_duration_frames) {
		float opacity = 1 - ((float) (ofGetFrameNum() - frame_loaded)) / (float) transition_duration_frames;

		float opacity_mapped = 1-pow(1-opacity, 3);

		ofSetColor(ofColor::white, (int) (255*opacity_mapped));
		ofDrawRectangle(0, 0, ofGetWidth(), ofGetHeight());
	}
}