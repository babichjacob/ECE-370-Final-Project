#include "UI.h"


UI::UI()
{
}


UI::~UI()
{
}

void UI::setup() {
	cout << "UI::setup: " << "about to load fonts" << endl;
	font_small.load(".MyTunes/fonts/Heebo/Heebo-Regular.ttf", font_small_size);
	font_medium.load(".MyTunes/fonts/Heebo/Heebo-Light.ttf", font_medium_size);
	font_large.load(".MyTunes/fonts/Heebo/Heebo-Thin.ttf", font_large_size);
	cout << "UI::setup: " << "fonts loaded" << endl;

	cout << "UI::setup: " << "about to load icons" << endl;
	previous_song_icon.load(".MyTunes/icons/previous.png");
	next_song_icon.load(".MyTunes/icons/next.png");
	rewind_icon.load(".MyTunes/icons/backward-01.png");
	forward_icon.load(".MyTunes/icons/forward-01.png");
	play_icon.load(".MyTunes/icons/play.png");
	pause_icon.load(".MyTunes/icons/pause.png");
	cout << "UI::setup: " << "fonts loaded" << endl;

	cool_gray_lightest = ofColor(248, 250, 252);
	cool_gray_lighter = ofColor(241, 245, 248);
	cool_gray_light = ofColor(218, 225, 231);
	cool_gray = ofColor(184, 194, 204);
	cool_gray_darker = ofColor(96, 111, 123);
	cool_gray_darkest = ofColor(61, 72, 82);

	play_zone.x = 0;
	play_zone.y = 0;
	play_zone.height = 120;

	columns.x = 0;
	columns.y = play_zone.y+play_zone.height;
	columns.height = 30;
}

void UI::update() {
	play_zone.width = ofGetWidth();
	columns.width = ofGetWidth();
}

void UI::draw() {
	// Don't draw a UI until the songs are loaded in
	if (frame_loaded == -1) return;

	// Dummy: pretend the UI is this blue rectangle
	ofSetColor(ofColor::blue);
	ofDrawRectangle(ofGetWidth() / 4, ofGetHeight() / 4, ofGetWidth() / 2, ofGetHeight() / 2);

	// Draw the play zone
	ofSetColor(cool_gray_light);
	ofDrawRectangle(play_zone);

	// Draw icons in the play zone
	// Note: all the images are white colored so that they can be tinted here
	ofSetColor(cool_gray_darker);

	previous_song_icon.draw(60, get_icon_baseline(play_zone, previous_song_icon));
	rewind_icon.draw(118, get_icon_baseline(play_zone, rewind_icon));

	// Because playing and pausing is the primary actions, make those the darkest drawn
	ofSetColor(cool_gray_darkest);
	// todo
	if (true) {
		play_icon.draw(184, get_icon_baseline(play_zone, play_icon));
	}
	else {
		pause_icon.draw(184, get_icon_baseline(play_zone, pause_icon));
	}

	ofSetColor(cool_gray_darker);
	forward_icon.draw(240, get_icon_baseline(play_zone, forward_icon));
	next_song_icon.draw(290, get_icon_baseline(play_zone, next_song_icon));


	// Draw the columns header
	ofSetColor(cool_gray_lightest);
	ofDrawRectangle(columns);

	// Draw the column edges (separator)
	ofSetColor(cool_gray_lighter);
	for (auto &column_edge : columns_edges) {
		if (column_edge != 0) ofDrawRectangle(column_edge, columns.y, 2, columns.height);
	}
	
	// Draw text on top of the header
	ofSetColor(cool_gray_darker);
	for (int i = 0; i < columns_entries.size(); i++) {
		font_medium.drawString(columns_entries[i], columns_edges[i] + font_medium_size, columns.y + columns.height/2.0 + font_medium_size/2.0);
	}

	// Do the fade-in animation
	if (ofGetFrameNum() - frame_loaded < transition_duration_frames) {
		// Opacity as a function of frames elapsed
		float opacity = 1 - ((float) (ofGetFrameNum() - frame_loaded)) / (float) transition_duration_frames;
		float opacity_mapped = 1-pow(1-opacity,5);

		// Cover the whole screen the same as the background color
		ofSetColor(ofGetBackgroundColor(), (int) (255*opacity_mapped));
		ofDrawRectangle(0, 0, ofGetWidth(), ofGetHeight());
	}
}


// Returns a y coordinate for aligning the icons vertically in the center of the play zone
int get_icon_baseline(ofRectangle play_zone, ofImage icon) {
	return (play_zone.height - play_zone.y - icon.getHeight()) / 2 + play_zone.y;
}