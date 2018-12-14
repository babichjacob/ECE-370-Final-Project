#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	ofSetBackgroundColor(ofColor::white);
	ofSetFrameRate(60);
	ofSetBackgroundAuto(true);

	srand(time(NULL));


	all_songs = find_all_songs();
	albums_map = build_albums(all_songs);
	artists_map = build_artists(albums_map);
	ui.frame_loaded = ofGetFrameNum();

	ui.setup();

	ui.all_songs = &all_songs;
	ui.albums_map = &albums_map;
	ui.artists_map = &artists_map;
}

//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw() {
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

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

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
