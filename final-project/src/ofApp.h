#pragma once

#include "Album.h"
#include "Artist.h"
#include "library.h"
#include "ofMain.h"
#include "Song.h"
#include "UI.h"


class ofApp : public ofBaseApp{
	UI ui;

	vector<Song> all_songs;
	map<string, Album> albums_map;
	map<string, Artist> artists_map;

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void mouseScrolled(int x, int y, float scrollX, float scrollY);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
		
};
