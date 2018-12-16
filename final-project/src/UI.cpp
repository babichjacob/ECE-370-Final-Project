#include "UI.h"


UI::UI()
{
}


UI::~UI()
{
}

void UI::setup() {
	cout << "UI::setup: " << "about to load fonts" << endl;
	font_md.load(".MyTunes/fonts/Heebo/Heebo-Light.ttf", font_md_size);
	font_xl.load(".MyTunes/fonts/Heebo/Heebo-Thin.ttf", font_xl_size);
	cout << "UI::setup: " << "done loading fonts" << endl;

	cout << "UI::setup: " << "about to load icons" << endl;

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

		// Initialize all icons as inactive
		this_icon_bundle.is_active = false;

		// Set the shortcut keys
		this_icon_bundle.shortcut_key = icon_shortcuts[i];

		// Finally, add the icon bundle to the map
		icons.insert(pair<string, IconBundle>(icon_name, this_icon_bundle));
	}
	cout << "UI::setup: " << "icons loaded" << endl;

	// Song view by default
	view_mode = view_song;

	// dummy
	currently_playing_song_image.load(".MyTunes/icons/image.png");

	// All constant values in UI elements
	play_zone.x = 0;
	play_zone.y = 0;
	play_zone.height = 125;

	currently_playing_zone.y = play_zone.y + padding_standard;
	currently_playing_zone.height = play_zone.height - padding_standard*2;

	song_slider_outer.height = 6;
	song_slider_inner.height = song_slider_outer.height;

	currently_playing_song_image.resize(currently_playing_zone.height - 2 * padding_standard, currently_playing_zone.height - 2 * padding_standard);

	columns.x = 0;
	columns.y = play_zone.y+play_zone.height;
	columns.height = 30;

	view_zone.x = 0;
}

void UI::windowResized() {
	play_zone.width = ofGetWidth();
	
	// Make the currently playing zone take up the middle half of the screen
	// (or up to the remaining space not occupied by the icons)
	currently_playing_zone.width = ofGetWidth() < 1700 ? ofGetWidth() - 400 - padding_standard : ofGetWidth() / 2;
	currently_playing_zone.x = ofGetWidth() < 1700 ? 400 : ofGetWidth() / 2 - currently_playing_zone.width / 2;

	// Update slider sizes and coordinates
	song_slider_outer.x = currently_playing_zone.x + padding_left_song_slider;
	song_slider_outer.y = currently_playing_zone.y + (currently_playing_zone.height - song_slider_outer.height)/2;
	song_slider_outer.width = (currently_playing_zone.width + currently_playing_zone.x - song_slider_outer.x) - padding_right_song_slider;

	song_slider_inner.x = song_slider_outer.x;
	song_slider_inner.y = song_slider_outer.y;

	// Make the columns header take up the entire width of the window
	columns.width = ofGetWidth();

	// Start the view zone below the columns header
	view_zone.y = columns.y + columns.height + columns_border_size;
	// Make it take up all the remaining space
	view_zone.height = ofGetHeight() - view_zone.y;
	// Make it take up the width of the whole window
	view_zone.width = ofGetWidth();

	// Recalculate the number of songs that can fit on-screen
	// (add 1 so that one can be partially cut off at the bottom of the screen, indicating that
	//  the user can scroll down to see more songs)
	songs_that_can_fit_on_screen = view_zone.height / song_entry_height + 1;

	// Reset the top song in the list to be certain there are no out-of-bounds errors
	top_song_in_list = 0;
}

void UI::draw_full(bool is_paused, Song song, ofSoundPlayer player) {
	// Don't draw a UI until the songs are loaded in
	// (see below for the fade-in animation that happens once loading is complete)
	if (frame_loaded == -1) return;

	// Draw the play zone
	draw_play_zone();

	// Draw icons in the play zone
	draw_icons(is_paused);

	// Draw the currently playing zone
	draw_currently_playing_zone(song, player);

	// Draw the columns header
	draw_columns_header();

	// Draw the view mode that matches view_mode
	switch (view_mode) {
	case view_song:
		draw_song_view();
		break;
	case view_album:
		draw_album_view();
		break;
	case view_artist:
		draw_artist_view();
		break;
	}

	// Do the fade-in animation only when necessary
	if (ofGetFrameNum() - frame_loaded < transition_duration_frames) {
		// Ranges from 0 to 1, where 0 is the very beginning and 1 is the end
		float time_progress = ((float)(ofGetFrameNum() - frame_loaded)) / (float)transition_duration_frames;
		draw_splash_screen(time_progress);
	}
}

void UI::draw_play_zone() {
	ofSetColor(cool_gray_light);
	ofDrawRectangle(play_zone);
}


void UI::draw_icons(bool is_paused) {
	// Draw icons in the play zone
	for (auto &icon_name_and_bundle : icons) {
		if (icon_name_and_bundle.first == "play") {
			if (!is_paused) continue;
		}
		else if (icon_name_and_bundle.first == "pause") {
			if (is_paused) continue;
		}

		icon_name_and_bundle.second.hitbox.y = get_icon_baseline(play_zone, icon_name_and_bundle.second.icon);

		// If the icon is being actively pressed (see ofApp::mousePressed)
		if (icon_name_and_bundle.second.is_active) {
			// Then show the active color instead
			ofSetColor(icon_name_and_bundle.second.color_active);
		}
		// Otherwise,
		else {
			// show the inactive color
			ofSetColor(icon_name_and_bundle.second.color_inactive);
		}

		// Finally, draw the icon where it belongs
		icon_name_and_bundle.second.icon.draw(icon_name_and_bundle.second.hitbox.x, icon_name_and_bundle.second.hitbox.y);
	}
}


void UI::draw_currently_playing_zone(Song song, ofSoundPlayer player) {
	// Draw the rounded rectangle background
	ofSetColor(cool_gray_lighter);
	ofDrawRectRounded(currently_playing_zone, currently_playing_zone_rounded_radius);

	// Draw the song information in the currently playing zone

	// If the player hasn't started yet, don't show anything yet
	if (!player.isPlaying()) return;

	// Cover art
	ofSetColor(ofColor::steelBlue);
	currently_playing_song_image.draw(currently_playing_zone.x + padding_standard, get_icon_baseline(play_zone, currently_playing_song_image));

	// Factor out this common x position of the title, album, and artist
	int currently_playing_text_x_pos = currently_playing_zone.x + 2 * padding_standard + currently_playing_song_image.getWidth();

	// Title
	ofSetColor(cool_black);
	font_md.drawString(song.title,  currently_playing_text_x_pos, currently_playing_zone.y + font_md_size + padding_standard);

	// Album
	ofSetColor(cool_gray_darker);
	font_md.drawString(song.album,  currently_playing_text_x_pos, currently_playing_zone.y + currently_playing_zone.height / 2 + font_md_size / 2);

	// Artist
	ofSetColor(cool_black);
	font_md.drawString(song.artist, currently_playing_text_x_pos, currently_playing_zone.y + currently_playing_zone.height - padding_standard);

	// Progress slider
	// Outer
	ofSetColor(cool_gray_light);
	ofDrawRectRounded(song_slider_outer, song_slider_outer.height / 2);

	// Inner
	ofSetColor(cool_gray);
	song_slider_inner.width = song_slider_outer.width * player.getPosition();
	ofDrawRectRounded(song_slider_inner, song_slider_inner.height / 2);

	// Time progress
	
	pair<int, int> since_start = convert_ms_to_min_and_sec(player.getPositionMS());

	// Calculate total song length
	int song_length = player.getPositionMS() / player.getPosition();
	// Determine the amount of time left
	int time_left = song_length - player.getPositionMS();
	// Convert total milliseconds to minutes and seconds
	pair<int, int> to_end = convert_ms_to_min_and_sec(time_left);

	// Format it so it's better human readable in the UI
	string since_start_colonized =  format_as_time(since_start.first, since_start.second);
	string to_end_colonized = "-" + format_as_time(to_end.first, to_end.second);

	// Draw them on screen
	int time_indicators_pos_y = song_slider_outer.y + song_slider_outer.height + padding_standard;

	ofSetColor(cool_gray);
	font_md.drawString(since_start_colonized, song_slider_outer.x                           - strlen(since_start_colonized.c_str())*font_md_size/3, time_indicators_pos_y);

	if (time_left >= 0) // There's a problem with this showing a really large negative number at the beginning of the song, so just don't draw it in those cases
	font_md.drawString(to_end_colonized,      song_slider_outer.x + song_slider_outer.width - strlen(since_start_colonized.c_str())*font_md_size/2, time_indicators_pos_y);
}


void UI::draw_columns_header() {
	// Draw the columns header
	ofSetColor(cool_gray_lightest);
	ofDrawRectangle(columns);

	// Draw the column edges (separator)
	ofSetColor(cool_gray_lighter);
	for (auto &column_edge : columns_edges) {
		if (column_edge != 0) ofDrawRectangle(column_edge, columns.y, columns_border_size, columns.height);
	}

	// Draw a bottom border on the columns header
	ofSetColor(cool_gray_lighter);
	ofDrawRectangle(columns.x, columns.y + columns.height, columns.width, columns_border_size);

	// Draw text on top of the columns header
	ofSetColor(cool_gray_darker);
	for (int i = 0; i < columns_entries.size(); i++) {
		font_md.drawString(columns_entries[i], columns_edges[i] + font_md_size, columns.y + columns.height / 2.0 + font_md_size / 2.0);
	}
}


void UI::draw_splash_screen(float time_progress) {
	// Opacity as a function of frames elapsed
	float opacity = 1 - pow(time_progress, 5);

	// Cover the whole screen the same as the background color (white)
	ofSetColor(ofGetBackgroundColor(), (int)(255 * opacity));
	ofDrawRectangle(0, 0, ofGetWidth(), ofGetHeight());

	// But show the title text in the center of the screen
	// This text fades out faster (it feels too strong otherwise)
	ofSetColor(cool_black, (int)(255 * pow(opacity, 5)));
	font_xl.drawString("MyTunes", ofGetWidth() / 2 - font_xl_size * strlen("MyTunes")/2, ofGetHeight() / 2 - font_xl_size/3);
}


void UI::draw_artist_view() {
	// todo
}


void UI::draw_album_view() {
	// todo
}


void UI::draw_song_view() {
	// Clear the view zone
	ofSetColor(ofGetBackgroundColor());
	ofDrawRectangle(view_zone);

	song_entries.clear();

	for (int j = 0; j < songs_that_can_fit_on_screen; j++) {
		int index = j + top_song_in_list;

		Song this_song = (*all_songs)[index];
		SongEntry this_song_entry;

		this_song_entry.index = index;

		this_song_entry.song = this_song;

		this_song_entry.hitbox.x = 0;
		this_song_entry.hitbox.y = view_zone.y + j*song_entry_height;
		this_song_entry.hitbox.height = song_entry_height;
		this_song_entry.hitbox.width = ofGetWidth();


		// Add the song entry to the list of song entries
		song_entries.push_back(this_song_entry);

		// Zebra striping
		ofSetColor(j % 2 == 0 ? ofColor::white : cool_gray_lightest);
		ofDrawRectangle(this_song_entry.hitbox);

		// Write out song info (text)
		ofSetColor(cool_gray_darkest);
		font_md.drawString(this_song.title,             columns_edges[0] + font_md_size, this_song_entry.hitbox.y + font_md_size + padding_song_entry);
		font_md.drawString(this_song.album,             columns_edges[1] + font_md_size, this_song_entry.hitbox.y + font_md_size + padding_song_entry);
		font_md.drawString(this_song.artist,            columns_edges[2] + font_md_size, this_song_entry.hitbox.y + font_md_size + padding_song_entry);
		font_md.drawString(this_song.genre,             columns_edges[3] + font_md_size, this_song_entry.hitbox.y + font_md_size + padding_song_entry);
		font_md.drawString(to_string(this_song.year),   columns_edges[4] + font_md_size, this_song_entry.hitbox.y + font_md_size + padding_song_entry);
	}
}


void UI::scroll_up() {
	top_song_in_list -= songs_to_scroll;
	// Ensure scrolling cannot go past the first song
	top_song_in_list = max(top_song_in_list, 0);
	// Undo the changes done below in `UI::scroll_down`
	songs_that_can_fit_on_screen = view_zone.height / song_entry_height + 1;
}


void UI::scroll_down() {
	top_song_in_list += songs_to_scroll;
	// Ensure scrolling cannot go past the last song
	int should_be_highest_scroll = all_songs->size() - songs_that_can_fit_on_screen;
	if (top_song_in_list > should_be_highest_scroll) {
		top_song_in_list = should_be_highest_scroll;
		// Since we've scrolled to the bottom, make sure there are no songs being cut off at the end.
		// (see `UI::windowResized()` for that)
		songs_that_can_fit_on_screen = view_zone.height / song_entry_height;
	}
}


string get_icon_path(string icon_name) {
	return (fs::path(ICONS_DIR) / icon_name).replace_extension(".png").string();
}


// Returns a y coordinate for aligning the icons vertically in the center of the play zone
int get_icon_baseline(ofRectangle play_zone, ofImage icon) {
	return (play_zone.height - play_zone.y - icon.getHeight()) / 2 + play_zone.y;
}


pair<int, int> convert_ms_to_min_and_sec(int milliseconds) {
	int total_seconds = milliseconds / 1000;

	// (minutes, seconds)
	return pair<int, int>(total_seconds / 60, total_seconds % 60);
}


string format_as_time(int minutes, int seconds) {
	string seconds_as_string;
	// Add on a zero in front if the number of seconds in a single digit
	if (seconds < 10) seconds_as_string = "0" + to_string(seconds);
	else seconds_as_string = to_string(seconds);

	return to_string(minutes) + ":" + seconds_as_string;
}