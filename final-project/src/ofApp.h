#pragma once

#include "Album.h"
#include "Artist.h"
#include "library.h"
#include "ofMain.h"
#include "preferences.h"
#include "search_engine.h"
#include "Song.h"
#include "UI.h"
#include "utils.h"


class ofApp : public ofBaseApp{
	UI ui;

	ofSoundPlayer player;

	fs::recursive_directory_iterator music_directory;

	// True until songs are done loading in
	bool is_loading = true;
	int loaded_index = 0;

	// As crazy as it sounds, ofSoundPlayer cannot / does not keep track 
	// of whether or not it's paused
	bool is_paused = true;

	Song* currently_playing_song = nullptr;

	// How much time is skipped by a forward or rewind
	int skip_time = 10;

	Preferences preferences;

	Songs all_songs;
	Albums albums_map;
	Artists artists_map;

	SearchEngine search_engine;

	vector<Song*> search_results;

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


		void start_playing(Song* song, int song_index);
};
