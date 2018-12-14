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

	// dummy
	currently_playing_song_image.load(".MyTunes/icons/image.png");

	cool_gray_lightest = ofColor(248, 250, 252);
	cool_gray_lighter = ofColor(241, 245, 248);
	cool_gray_light = ofColor(218, 225, 231);
	cool_gray = ofColor(184, 194, 204);
	cool_gray_darker = ofColor(96, 111, 123);
	cool_gray_darkest = ofColor(61, 72, 82);
	cool_black = ofColor(34, 41, 47);

	play_zone.x = 0;
	play_zone.y = 0;
	play_zone.height = 125;

	currently_playing_zone.y = play_zone.y + padding_standard;
	currently_playing_zone.height = play_zone.height - padding_standard*2;

	currently_playing_song_image.resize(currently_playing_zone.height - 2 * padding_standard, currently_playing_zone.height - 2 * padding_standard);

	columns.x = 0;
	columns.y = play_zone.y+play_zone.height;
	columns.height = 30;
}

void UI::update() {
	play_zone.width = ofGetWidth();
	
	// Make the currently play zone take up the middle half of the screen
	// (or up to the remaining space not occupied by the icons)
	currently_playing_zone.width = ofGetWidth() < 1700 ? ofGetWidth() - 400 - 17 : ofGetWidth() / 2;
	currently_playing_zone.x = ofGetWidth() < 1700 ? 400 : ofGetWidth() / 2 - currently_playing_zone.width / 2;

	columns.width = ofGetWidth();
}

void UI::draw() {
	// Don't draw a UI until the songs are loaded in
	// (see below for the fade-in animation that happens once loading is complete)
	if (frame_loaded == -1) return;

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
	next_song_icon.draw(298, get_icon_baseline(play_zone, next_song_icon));

	// Draw the currently playing zone
	ofSetColor(cool_gray_lighter);
	ofDrawRectRounded(currently_playing_zone, 9);

	// Draw the song information in the currently playing zone
	ofSetColor(ofColor::steelBlue);
	currently_playing_song_image.draw(currently_playing_zone.x + padding_standard, get_icon_baseline(play_zone, currently_playing_song_image));
	
	int currently_playing_text_x_pos = currently_playing_zone.x + 2 * padding_standard + currently_playing_song_image.getWidth();

	ofSetColor(cool_black);
	font_medium.drawString("Distant Lovers", currently_playing_text_x_pos, currently_playing_zone.y + font_medium_size + padding_standard);

	ofSetColor(cool_gray_darker);
	font_medium.drawString("Birth of a New Day", currently_playing_text_x_pos, currently_playing_zone.y + currently_playing_zone.height/2 + font_medium_size/2);

	ofSetColor(cool_black);
	font_medium.drawString("2814", currently_playing_text_x_pos, currently_playing_zone.y + currently_playing_zone.height - padding_standard);

	// Draw the columns header
	ofSetColor(cool_gray_lightest);
	ofDrawRectangle(columns);

	// Draw the column edges (separator)
	ofSetColor(cool_gray_lighter);
	for (auto &column_edge : columns_edges) {
		if (column_edge != 0) ofDrawRectangle(column_edge, columns.y, 2, columns.height);
	}
	
	// Draw text on top of the columns header
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