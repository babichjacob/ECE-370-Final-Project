#include "ofMain.h"
#include "ofApp.h"

//========================================================================
int main( ){
	// Default, dummy size. Replaced two lines below.
	ofSetupOpenGL(1280,720,OF_WINDOW);			// <-------- setup the GL context

	// Top left
	ofSetWindowPosition(0, 0);

	// Full size (not to be confused with full-screen or maximized because it is neither of these)
	ofSetWindowShape(ofGetScreenWidth(), ofGetScreenHeight());

	// this kicks off the running of my app
	// can be OF_WINDOW or OF_FULLSCREEN
	// pass in width and height too:
	ofRunApp(new ofApp());
}
