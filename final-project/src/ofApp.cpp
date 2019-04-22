#include "ofApp.h"



//--------------------------------------------------------------
void ofApp::setup(){
	ofSetBackgroundColor(ofColor::white);
	ofSetFrameRate(60);
	ofSetBackgroundAuto(false);

	preferences = preferences_init();
	ui.preferences = &preferences;

	// Set the player volume to the user's preferred volume, or 0.875 if they haven't picked one yet
	player.setVolume(stof(preferences_get(preferences, "volume", "0.875")));

	// Set the music directory iterator that will be exhausted once all songs are loaded
	music_directory = fs::recursive_directory_iterator(MUSIC_DIR);

	ui.setup();
	ui.frame_loaded = 0;
}

//--------------------------------------------------------------
void ofApp::update(){
	// This is done in update so that there is an openFrameworks window
	// open and available (otherwise it'll be confusing to the user why
	// their program isn't opening when it's loading)
	// Also, the first 4 frames have to be skipped because those never get painted on screen for some reason
	// (I think the window isn't visible until the 5th frame, at least on Windows 10)

	if (is_loading) {
		is_loading = find_all_songs_incrementally(&all_songs, &music_directory, &loaded_index);

		albums_map = build_albums(all_songs);
		artists_map = build_artists(albums_map);
		sort_songs(&all_songs, ui.sorted_by);

		ui.all_songs = &all_songs;
		ui.albums_map = &albums_map;
		ui.artists_map = &artists_map;
	}

	// Start playing the next song when the current one finishes
	// (less than 80 milliseconds left I guess)
	if (player.isPlaying() && player.getPosition() != 0) {
		int song_length_ms = round(player.getPositionMS() / player.getPosition());
		if (song_length_ms - player.getPositionMS() <= 80) {
			// When the song finishes, add 1 to its play count
			all_songs[index_of_currently_playing_song]->increment_plays();
			// Because the play count was just changed, re-draw the song view if it's sorted by play count in any way
			if (ui.view_mode == view_song && vector_contains(ui.sorted_by, static_cast<string>("plays"))) {
				sort_songs(&all_songs, ui.sorted_by);
				ui.draw_song_view();
			}

			cout << "next song coming up" << endl;
			keyPressed(OF_KEY_RIGHT);
		}
	}
}

//--------------------------------------------------------------
void ofApp::draw() {
	if (!is_loading && all_songs.size() == 0) {
		ui.draw_splash_screen(0);
		ui.draw_no_songs();
	}
	else {
		ui.draw_full(is_paused, currently_playing_song, player);
	}
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	// Do not acknowledge any key presses if the library is empty
	if (all_songs.size() == 0) return;

	// When the spacebar is pressed,
	if (key == ' ') {
		// When someone hits the spacebar or the play icon for the first time,
		// nothing will be loaded. So in that case, just start playing the first
		// song in the library.
		if (!player.isPlaying()) {
			start_playing(all_songs[0], 0);
			return;
		}

		// Switch between playing and paused
		is_paused = !is_paused;
		// And update that on the player
		player.setPaused(is_paused);
	}
	// When the left arrow is pressed,
	if (key == OF_KEY_LEFT) {
		// Go back a song
		int previous_index = index_of_currently_playing_song - 1;
		// Or loop around to the last song in the library
		if (previous_index < 0) previous_index = all_songs.size() - 1;
		start_playing(all_songs[previous_index], previous_index);
	}
	// When the right arrow is pressed,
	if (key == OF_KEY_RIGHT) {
		// Go forward a song
		unsigned int next_index = index_of_currently_playing_song + 1;
		// Or loop around to the first song in the library
		if (next_index >= static_cast<unsigned int>(all_songs.size())) next_index = 0;
		start_playing(all_songs[next_index], next_index);
	}
	// When the comma key is pressed,
	if (key == ',') {
		// Rewind `skip_time` seconds
		player.setPositionMS(max(0, player.getPositionMS() - skip_time * 1000));
	}
	// When the period key is pressed,
	if (key == '.') {
		// Fast forward `skip_time` seconds
		int song_length_ms = round(player.getPositionMS() / player.getPosition());
		player.setPositionMS(min(song_length_ms, player.getPositionMS() + skip_time * 1000));
	}
	// When the down arrow is pressed,
	if (key == OF_KEY_DOWN) {
		// Scroll down
		ui.scroll_down();
	}
	// When the up arrow is pressed,
	if (key == OF_KEY_UP) {
		// Scroll up
		ui.scroll_up();
	}
	// When the - key is pressed,
	if (key == '-') {
		// Lower the volume (without going past zero)
		player.setVolume(max(0.0F, player.getVolume() - 0.1F));
	}
	// When the + key (shared with =) is pressed,
	if (key == '=') {
		// Increase the volume (without going past 1)
		player.setVolume(min(1.0F, player.getVolume() + 0.1F));
	}

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
	for (auto &icons_entry : ui.icons) {
		// Reset every icon to inactive
		icons_entry.second.is_active = false;
	}

	// If a click is in the play zone,
	if (ui.play_zone.inside(x, y)) {
		// And inside the currently playing zone,
		if (ui.currently_playing_zone.inside(x, y)) {
			// And inside the song slider,
			// (but add on 6 pixels on the top and bottom to make it easier to click on)
			ofRectangle more_generous_slider_hitbox;

			more_generous_slider_hitbox.x = ui.song_slider_outer.x;
			more_generous_slider_hitbox.width = ui.song_slider_outer.width;

			more_generous_slider_hitbox.y = ui.song_slider_outer.y - 6;
			more_generous_slider_hitbox.height = ui.song_slider_outer.height + 12;

			if (more_generous_slider_hitbox.inside(x, y)) {
				// Then update the song progress to match the click.
				// Calculate what progress in the song this matches
				float progress = (x - ui.song_slider_outer.x) / ui.song_slider_outer.width;
				// And update it on the player
				player.setPosition(progress);
			}

			// Either the song slider was clicked on above and it was properly handled,
			// or nothing at all was clicked on.
			// We're done here. Return.
			return;
		}
		// But if not inside the currently playing zone
		else {
			// Check if it's within the volume slider
			// (but add on 6 pixels on the left and right to make it easier to click on)
			ofRectangle more_generous_slider_hitbox;

			more_generous_slider_hitbox.y = ui.volume_slider_outer.y;
			more_generous_slider_hitbox.height = ui.volume_slider_outer.height;

			more_generous_slider_hitbox.x = ui.volume_slider_outer.x - 6;
			more_generous_slider_hitbox.width = ui.volume_slider_outer.width + 12;

			if (more_generous_slider_hitbox.inside(x, y)) {
				// Then update the volume to match the click.
				float volume = (ui.volume_slider_outer.y + ui.volume_slider_outer.height - y) / ui.volume_slider_outer.height;

				// And update it on the player
				player.setVolume(volume);

				// And the preferences
				preferences_set(preferences, "volume", to_string(volume));
			}

			// Check each icon to see if the click is inside it
			for (auto& icons_entry : ui.icons) {
				if (icons_entry.second.hitbox.inside(x, y)) {
					icons_entry.second.is_active = true;

					// Simulate the key press the icon is a shortcut for
					keyPressed(icons_entry.second.shortcut_key);

					// We found the one that's being pressed. Don't check any others. 
					// Necessary to prevent double-detection of play and pause
					// because their hitboxes overlap.
					return;
				}
			}
		}
	}
	// Or if a click is inside the columns header
	else if (ui.columns.inside(x, y)) {
		for (int i = 0, n = ui.columns_entries.size(); i < n; i++) {
			int left_bound = ui.columns_edges[i];
			int right_bound = (i == n-1) ? INT_MAX : ui.columns_edges[i+1];
			// And the click happens in a particular header
			if (left_bound <= x && x < right_bound) {
				// Add that to the sort mode

				string column_header_name = ui.columns_entries[i];
				string column_as_key;

				// Artist and album are omitted because those are how you open the album view
				if (column_header_name == u8"♥") column_as_key = "is_favorited";
				else if (column_header_name == "Song Name") column_as_key = "title";
				else if (column_header_name == "Genre") column_as_key = "genre";
				else if (column_header_name == "Year") column_as_key = "year";
				else if (column_header_name == "Plays") column_as_key = "plays";

				ui.add_to_sort_order(column_as_key);

				return;
			}
		}
	}
	// Or if a click is in the view zone,
	else if (ui.view_zone.inside(x, y)) {
		// And the view mode is to view songs,
		if (ui.view_mode == view_song) {
			// Then check each song entry for a click
			for (auto& song_entry : ui.song_entries) {
				// And if that's where the click happened
				if (song_entry.hitbox.inside(x, y)) {
					// Either toggle that song being favorited
					if (x <= song_entry.is_favorited_x_limit) {
						song_entry.media->toggle_favorited();
						// If there is any kind of sorting done on favorites, re-sort the view
						if (ui.view_mode == view_song && vector_contains(ui.sorted_by, static_cast<string>("is_favorited"))) {
							sort_songs(&all_songs, ui.sorted_by);
							ui.draw_song_view();
						}
					}
					// or play the song
					else start_playing(song_entry.media, song_entry.index);
					// We found it. Don't check any others.
					return;
				}
			}
		}
	}
}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

void ofApp::mouseScrolled(int x, int y, float scrollX, float scrollY) {
	if (scrollY == +1) {
		ui.scroll_up();
	}
	else if (scrollY == -1) {
		ui.scroll_down();
	}
}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){
	ui.windowResized();
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}


void ofApp::start_playing(Song* song, int song_index) {
	song->print();

	index_of_currently_playing_song = song_index;
	currently_playing_song = song;

	// Load the artwork into the image
	ui.currently_playing_song_image.load(song->artwork_file_path.string());
	// And make sure it's the proper size
	ui.resize_artwork();

	// Never start a song paused
	is_paused = false;
	player.setPaused(false);
	// The second parameter determines whether or not the player can stream directly from the file
	// without loading it in its entirety first (much faster)
	player.load(song->music_file_path.string(), true);
	
	player.play();
}