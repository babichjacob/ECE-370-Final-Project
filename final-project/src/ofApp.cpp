#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	ofSetBackgroundColor(ofColor::white);
	ofSetFrameRate(60);
	ofSetBackgroundAuto(false);

	ui.setup();
}

//--------------------------------------------------------------
void ofApp::update(){
	// This is done in update so that there is an openFrameworks window
	// open and available (otherwise it'll be confusing to the user why
	// their program isn't opening when it's loading)
	// Also, the first 4 frames have to be skipped because those never get painted on screen for some reason
	// (I think the window isn't visible until the 5th frame, at least on Windows 10)
	if (ofGetFrameNum() == 4) {
		all_songs = find_all_songs();
		albums_map = build_albums(all_songs);
		artists_map = build_artists(albums_map);
		all_songs = rebuild_songs(artists_map, all_songs.size());
		ui.frame_loaded = ofGetFrameNum();

		ui.all_songs = &all_songs;
		ui.albums_map = &albums_map;
		ui.artists_map = &artists_map;
	}

	// Start playing the next song when the current one finishes
	// (less than 80 milliseconds left I guess)
	if (player.isPlaying() && player.getPosition() != 0) {
		int song_length_ms = round(player.getPositionMS() / player.getPosition());
		if (song_length_ms - player.getPositionMS() <= 80) {
			cout << "next song coming up" << endl;
			keyPressed(OF_KEY_RIGHT);
		}
	}
}

//--------------------------------------------------------------
void ofApp::draw() {
	if (ofGetFrameNum() < 5) ui.draw_splash_screen(0);
	else if (all_songs.size() == 0) {
		ui.draw_splash_screen(0); 
		ui.draw_no_songs();
	}
	else ui.draw_full(is_paused, all_songs[index_of_currently_playing_song], player);
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
		int next_index = index_of_currently_playing_song + 1;
		// Or loop around to the first song in the library
		if (next_index >= all_songs.size()) next_index = 0;
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

				cout << "setting volume to " << volume << endl;
				// And update it on the player
				player.setVolume(volume);
			}

			// Check each icon to see if the click is inside it
			for (auto &icons_entry : ui.icons) {
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
	// Or if a click is in the view zone,
	else if (ui.view_zone.inside(x, y)) {
		// And the view mode is to view songs,
		if (ui.view_mode == view_song) {
			// Then check each song entry for a click
			for (auto &song_entry : ui.song_entries) {
				if (song_entry.hitbox.inside(x, y)) {
					// And play the song if that's where the click happened
					start_playing(song_entry.song, song_entry.index);
					// We found it. Don't check any others.
					return;
				}
			}
		}
	}
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
	for (auto &icons_entry : ui.icons) {
		// Reset every icon to inactive
		icons_entry.second.is_active = false;
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


void ofApp::start_playing(Song song, int song_index) {
	song.print();

	index_of_currently_playing_song = song_index;

	// Load the artwork into the image
	ui.currently_playing_song_image.load(song.artwork_file_path.string());
	// And make sure it's the proper size
	ui.resize_artwork();

	// Never start a song paused
	is_paused = false;
	player.setPaused(false);
	// The second parameter determines whether or not the player can stream directly from the file
	// without loading it in its entirety first (much faster)
	player.load(song.music_file_path.string(), true);
	
	player.play();
}