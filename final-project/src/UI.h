#pragma once

#include <filesystem>

#include "ofMain.h"
#include "Album.h"
#include "Artist.h"
#include "Song.h"

constexpr auto ICONS_DIR = "bin/data/.MyTunes/icons";

namespace fs = std::experimental::filesystem;

using std::map;
using std::max;
using std::min;


// Whether to display a list of songs, albums, or artists
enum ViewMode { view_song, view_album, view_artist };


// An icon set is an ofImage and other stuff relating to it
struct IconBundle {
	ofImage icon;
	ofRectangle hitbox;
	ofColor color_inactive;
	ofColor color_active;

	// Whether the icon is currently being pressed or not
	bool is_active = false;
};

typedef struct IconBundle IconBundle;


class UI
{
public:
	int frame_loaded = -1;

	// See enum above
	ViewMode view_mode;

	Artists *artists_map;
	Albums *albums_map;
	Songs *all_songs;

	// Icons (public -- necessary because of click detection in ofApp)
	map<string, IconBundle> icons;

	// Zones with clickable elements (public -- necessary because of click detection in ofApp)
	ofRectangle play_zone;
	ofRectangle currently_playing_zone;
	ofRectangle view_zone;


	UI();
	~UI();
	void setup();
	void windowResized();
	void draw();

	void draw_splash_screen(float time_progress);

	void draw_artist_view();
	void draw_album_view();
	void draw_song_view();

	void scroll_up();
	void scroll_down();

private:
	// Color palette
	ofColor cool_gray_lightest = ofColor(248, 250, 252);
	ofColor cool_gray_lighter = ofColor(241, 245, 248);
	ofColor cool_gray_light = ofColor(218, 225, 231);
	ofColor cool_gray = ofColor(184, 194, 204);
	ofColor cool_gray_darker = ofColor(96, 111, 123);
	ofColor cool_gray_darkest = ofColor(61, 72, 82);
	ofColor cool_black = ofColor(34, 41, 47);
	// End color palette

	// Fonts
	ofTrueTypeFont font_md;
	int font_md_size = 13;
	
	ofTrueTypeFont font_xl;
	int font_xl_size = 96;
	// End fonts

	// How long the fade-in animation will last
	// 45 frames / 60 frames per second = 0.75 seconds
	int transition_duration_frames = 45;

	// How many songs are scrolled into / out of view when scrolling
	int songs_to_scroll = 5;

	// The standard amount of padding inside / around elements
	int padding_standard = 16;

	// Icons (private information before bundling)
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
	// End icons

	
	// dummy
	ofImage currently_playing_song_image;
	// end dummy

	// Columns
	ofRectangle columns;
	vector<string> columns_entries = {"Song Name", "Album", "Artist", "Genre", "Year"};
	vector<int> columns_edges = {0, 550, 950, 1350, 1600};
	int columns_border_size = 2;
	// End columns

	// Views

	// Song view
	// Index of the top-most song in the list when in song view
	int top_song_in_list = 0;

	int padding_song_entry = padding_standard / 2;
	int song_entry_height = font_md_size + padding_song_entry * 2;

	// Will be calculated from the height in the view space and the height of each song entry
	int songs_that_can_fit_on_screen;
	// End song view
};


string get_icon_path(string icon_name);
int get_icon_baseline(ofRectangle play_zone, ofImage icon);