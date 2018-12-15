#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	ofSetBackgroundColor(ofColor::white);
	ofSetFrameRate(60);
	ofSetBackgroundAuto(true);

	ui.setup();
}

//--------------------------------------------------------------
void ofApp::update(){
	// This is done in update so that there is an openFrameworks window
	// open and available (otherwise it'll be confusing to the user why
	// their program isn't opening when it's loading)
	// Also, the first 4 frames have to be skipped because those never get painted on screen for some reason
	// (I think the window isn't visible until the 5th frame)
	if (ofGetFrameNum() == 4) {
		all_songs = find_all_songs();
		albums_map = build_albums(all_songs);
		artists_map = build_artists(albums_map);
		ui.frame_loaded = ofGetFrameNum();

		ui.all_songs = &all_songs;
		ui.albums_map = &albums_map;
		ui.artists_map = &artists_map;
	}
}

//--------------------------------------------------------------
void ofApp::draw() {
	if (ofGetFrameNum() < 5) ui.draw_splash_screen(0);
	ui.draw();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

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
		// Check each icon to see if the click is inside it
		for (auto &icons_entry : ui.icons) {
			// True when the click is inside the icon's hitbox, false otherwise
			// (serves to reset previously clicked icons)
			icons_entry.second.is_active = (bool)(icons_entry.second.hitbox.inside(x, y));
		}
	} else if (ui.view_zone.inside(x, y)) {
		// todo: check against each song
	}
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
	for (auto &icons_entry : ui.icons) {
		// True when the click is inside the icon's hitbox, false otherwise
		// (serves to reset previously clicked icons)
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
