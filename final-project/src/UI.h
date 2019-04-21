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

	// Which key the icon is a shortcut for
	int shortcut_key;
};

typedef struct IconBundle IconBundle;


template <class T>
struct MediaEntry {
	int index;
	T media;
	ofRectangle hitbox;

	// Specific to songs
	int is_favorited_x_limit;
};


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

	// Sliders for volume and song progress (public -- necessary because of click detection in ofApp)
	ofRectangle song_slider_inner;
	ofRectangle song_slider_outer;

	ofRectangle volume_slider_inner;
	ofRectangle volume_slider_outer;

	// A vector of all the song, album, or artist entries visible on-screen at the moment 
	// (public -- necessary because of click detection in ofApp)
	vector<MediaEntry<Song>> song_entries;
	vector<MediaEntry<Album>> album_entries;
	vector<MediaEntry<Artist>> artist_entries;

	// The image element for the currently playing song's artwork
	ofImage currently_playing_song_image;


	UI();
	~UI();
	void setup();
	void windowResized();

	void draw_full(bool is_paused, Song song, ofSoundPlayer player);

	void draw_play_zone();
	void draw_icons(bool is_paused);
	void draw_volume_slider(ofSoundPlayer player);
	
	void resize_artwork();

	void draw_currently_playing_zone(Song song, ofSoundPlayer player);

	void draw_columns_header();

	void draw_artist_view();
	void draw_album_view();
	void draw_song_view();

	void draw_splash_screen(float time_progress);
	void draw_no_songs();

	void scroll_up();
	void scroll_down();

private:
	// Color palette
	ofColor cool_gray_lightest = ofColor(248, 250, 252);
	ofColor cool_gray_lighter = ofColor(241, 245, 248);
	ofColor cool_gray_light = ofColor(218, 225, 231);
	ofColor cool_gray = ofColor(184, 194, 204);
	ofColor cool_gray_dark = ofColor(135, 149, 161);
	ofColor cool_gray_darker = ofColor(96, 111, 123);
	ofColor cool_gray_darkest = ofColor(61, 72, 82);
	ofColor cool_black = ofColor(34, 41, 47);
	// End color palette

	// Fonts
	ofTrueTypeFont font_md;
	int font_md_size = 13;

	ofTrueTypeFont font_lg;
	int font_lg_size = 36;
	
	ofTrueTypeFont font_xl;
	int font_xl_size = 96;

	ofTrueTypeFont font_md_unicode;
	int font_md_unicode_size = 13;
	// End fonts

	// How long the fade-in animation will last
	// 30 frames / 60 frames per second = 0.5 seconds
	int transition_duration_frames = 30;

	// How many songs are scrolled into / out of view when scrolling
	int songs_to_scroll = 5;

	// The standard amount of padding inside / around elements
	int padding_standard = 16;

	// How far from the left side of the currently playing zone the song slider starts at
	int padding_left_song_slider = 400;
	// And how far from the right it ends
	int padding_right_song_slider = 100;

	// The radius of the rounded corners on the currently playing zone
	int currently_playing_zone_rounded_radius = 9;

	// Icons (private information before bundling)
	// The name of each icon (also corresponds to a file in the `.MyTunes/icons` folder)
	vector<string> icon_names = { "previous", "backward", "play", "pause", "forward", "next" };
	// These are eyeballed values (it's an aesthetics thing)
	vector <int> hitboxes_top_left_x = { 60, 118, 184, 180, 240, 298 };
	// Colors when there is nothing being done to the icon
	// Note: since play / pause is the primary action, it is emphasized by being slightly darker
	// Note: all the images are white colored in their file so that they can be tinted here
	vector<ofColor> icon_colors_inactive = { cool_gray_darker, cool_gray_darker, cool_gray_darkest, cool_gray_darkest, cool_gray_darker, cool_gray_darker };
	// Colors when the icon is being clicked on (aka within the range of the hitbox -- the whole point of creating such a thing)
	vector<ofColor> icon_colors_active = { cool_gray_darkest, cool_gray_darkest, cool_black, cool_black, cool_gray_darkest, cool_gray_darkest };
	// What each key an icon is a shortcut for
	vector<int> icon_shortcuts = { OF_KEY_LEFT, ',', ' ', ' ', '.', OF_KEY_RIGHT };
	// End icons

	// Columns
	ofRectangle columns;
	vector<string> columns_entries = {"", "Song Name", "Album", "Artist", "Genre", "Year", "Plays"};
	vector<int> columns_edges = {0, 24, 570, 970, 1370, 1620, 1700};
	int columns_border_size = 2;
	// End columns

	// Views

	// Song view
	// Index of the top-most song in the list when in song view
	int top_song_in_list = 0;

	int padding_song_entry = padding_standard*0.625;
	int song_entry_height = font_md_size + padding_song_entry * 2;

	// Will be calculated from the height in the view space and the height of each song entry
	int songs_that_can_fit_on_screen;
	// End song view
};


// Helper functions
string get_icon_path(string icon_name);
int get_icon_baseline(ofRectangle play_zone, ofImage icon);
pair<int, int> convert_ms_to_min_and_sec(int milliseconds);
string format_as_time(int minutes, int seconds);