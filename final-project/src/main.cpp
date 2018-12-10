#include "Artist.h"
#include "Album.h"
#include "library.h"
#include "ofMain.h"
#include "ofApp.h"
#include "ID3v2.h"
#include "Song.h"


//========================================================================
int main( ){
	cout << "main: begin program" << endl;

	cout << "main: testing - switching to mp3_main now" << endl;

	mp3_main();

	cout << "main: testing - mp3_main exited and the program will too now" << endl;
	return EXIT_SUCCESS;

	// Default, dummy size. Replaced two lines below.
	ofSetupOpenGL(1280,720,OF_WINDOW);			// <-------- setup the GL context

	// Top left
	ofSetWindowPosition(0, 0);

	// Full size (not to be confused with full-screen or maximized because it is neither of these)
	ofSetWindowShape(ofGetScreenWidth(), ofGetScreenHeight());

	// this kicks off the running of my app
	// can be OF_WINDOW or OF_FULLSCREEN
	// pass in width and height too:
	cout << "main: running openFrameworks app" << endl;
	ofRunApp(new ofApp());
}
