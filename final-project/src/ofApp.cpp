#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	ofSetBackgroundColor(ofColor::white);
	ofSetFrameRate(60);

	srand(time(NULL));

	ui.setup();
}

//--------------------------------------------------------------
void ofApp::update(){
	ui.update();

	// Spread loading across 3 frames (so we could potentially display a loading animation alongside)
	switch (ofGetFrameNum()) {
	case 1:
		all_songs = find_all_songs();
		break;
	case 2:
		albums_map = build_albums(all_songs);
		break;
	case 3:
		artists_map = build_artists(albums_map);
		break;
	case 4:
		ui.frame_loaded = ofGetFrameNum();
		break;
	}
}

//--------------------------------------------------------------
void ofApp::draw(){
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
	ofSetBackgroundColor(rand() % 256, rand() % 256, rand() % 256);
}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
