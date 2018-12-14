#include "UI.h"


UI::UI()
{
}


UI::~UI()
{
}

void UI::setup() {
	cout << "UI::setup: " << "(starting up)" << "about to set up colors" << endl;
	cool_gray_lightest = ofColor(248, 250, 252);
	cool_gray_lighter = ofColor(241, 245, 248);
	cool_gray_light = ofColor(218, 225, 231);
	cool_gray = ofColor(184, 194, 204);
	cool_gray_darker = ofColor(96, 111, 123);
	cool_gray_darkest = ofColor(61, 72, 82);
	cool_black = ofColor(34, 41, 47);
	cout << "UI::setup: " << "done setting up colors" << endl;

	cout << "UI::setup: " << "about to load fonts" << endl;
	font_small.load(".MyTunes/fonts/Heebo/Heebo-Regular.ttf", font_small_size);
	font_medium.load(".MyTunes/fonts/Heebo/Heebo-Light.ttf", font_medium_size);
	font_large.load(".MyTunes/fonts/Heebo/Heebo-Thin.ttf", font_large_size);
	cout << "UI::setup: " << "done loading fonts" << endl;

	cout << "UI::setup: " << "about to load icons" << endl;
	// The name of each icon (also corresponds to a file in the `.MyTunes/icons` folder)
	vector<string> icon_names = { "previous", "backward", "play", "pause", "forward", "next" };
	// These are eyeballed values (it's an aesthetics thing)
	vector <int> hitboxes_top_left_x = { 60, 118, 184, 184, 240, 298 };
	// Colors when there is nothing being done to the icon
	// Note: since play / pause is the primary action, it is emphasized by being slightly darker
	// Note: all the images are white colored in their file so that they can be tinted here
	vector<ofColor> icon_colors_inactive = { cool_gray_darker, cool_gray_darker, cool_gray_darkest, cool_gray_darkest, cool_gray_darker, cool_gray_darker };
	// Colors when the icon is being clicked on (aka within the range of the hitbox -- the whole point of creating such a thing)
	vector<ofColor> icon_colors_active = { cool_gray_darkest, cool_gray_darkest, cool_black, cool_black, cool_gray_darkest, cool_gray_darkest };

	// Create IconBundles for all the icons we need
	for (int i = 0; i < icon_names.size(); i++) {
		string icon_name = icon_names[i];

		IconBundle this_icon_bundle;

		// Load the image
		this_icon_bundle.icon.load(get_icon_path(icon_name));

		// Update the hitbox
		this_icon_bundle.hitbox.x = hitboxes_top_left_x[i];
		// The y value will be overwritten later
		this_icon_bundle.hitbox.y = 0;
		// Update the hitbox to match the size of the image
		this_icon_bundle.hitbox.width = this_icon_bundle.icon.getWidth();
		this_icon_bundle.hitbox.height = this_icon_bundle.icon.getHeight();

		// Set the inactive and active colors
		this_icon_bundle.color_inactive = icon_colors_inactive[i];
		this_icon_bundle.color_active = icon_colors_active[i];

		// Finally, add the icon bundle to the map
		icons.insert(pair<string, IconBundle>(icon_name, this_icon_bundle));
	}
	cout << "UI::setup: " << "icons loaded" << endl;


	cout << "UI::setup: " << "fonts loaded" << endl;

	// dummy
	currently_playing_song_image.load(".MyTunes/icons/image.png");

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
	currently_playing_zone.width = ofGetWidth() < 1700 ? ofGetWidth() - 400 - padding_standard : ofGetWidth() / 2;
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
	for (auto &icon_name_and_bundle : icons) {
		if (icon_name_and_bundle.first == "play") {
			// todo
		}
		else if (icon_name_and_bundle.first == "pause") {
			// todo (for now just draw the play icon)
			continue;
		}

		icon_name_and_bundle.second.hitbox.y = get_icon_baseline(play_zone, icon_name_and_bundle.second.icon);
		
		// If the mouse is being pressed, and it's within the bounds of the hitbox (our icon)
		if (ofGetMousePressed() && (icon_name_and_bundle.second.hitbox.inside(ofGetMouseX(), ofGetMouseY()))) {
			// Then show the active color instead
			ofSetColor(icon_name_and_bundle.second.color_active);

			// todo: and trigger the pressed event
		}
		// Otherwise,
		else {
			// show the inactive color
			ofSetColor(icon_name_and_bundle.second.color_inactive);
		}

		// Finally, draw the icon where it belongs
		icon_name_and_bundle.second.icon.draw(icon_name_and_bundle.second.hitbox.x, icon_name_and_bundle.second.hitbox.y);
	}

	// Draw the currently playing zone
	ofSetColor(cool_gray_lighter);
	ofDrawRectRounded(currently_playing_zone, 9);

	// Draw the song information in the currently playing zone
	ofSetColor(ofColor::steelBlue);
	currently_playing_song_image.draw(currently_playing_zone.x + padding_standard, get_icon_baseline(play_zone, currently_playing_song_image));
	
	int currently_playing_text_x_pos = currently_playing_zone.x + 2 * padding_standard + currently_playing_song_image.getWidth();

	// dummy
	ofSetColor(cool_black);
	font_medium.drawString("Distant Lovers", currently_playing_text_x_pos, currently_playing_zone.y + font_medium_size + padding_standard);

	ofSetColor(cool_gray_darker);
	font_medium.drawString("Birth of a New Day", currently_playing_text_x_pos, currently_playing_zone.y + currently_playing_zone.height/2 + font_medium_size/2);

	ofSetColor(cool_black);
	font_medium.drawString("2814", currently_playing_text_x_pos, currently_playing_zone.y + currently_playing_zone.height - padding_standard);
	// end dummy

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


string get_icon_path(string icon_name) {
	return (fs::path(ICONS_DIR) / icon_name).replace_extension(".png").string();
}


// Returns a y coordinate for aligning the icons vertically in the center of the play zone
int get_icon_baseline(ofRectangle play_zone, ofImage icon) {
	return (play_zone.height - play_zone.y - icon.getHeight()) / 2 + play_zone.y;
}