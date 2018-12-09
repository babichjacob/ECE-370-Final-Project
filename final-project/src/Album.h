#include "Song.h"

class Album
{
public:
	string name;
	vector<Song> songs;

	Album();
	~Album();
	Album(string name);
};
