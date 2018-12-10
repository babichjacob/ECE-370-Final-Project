#include "Song.h"

using std::count;
using std::cout;
using std::endl;
using std::pair;

class Album
{
public:
	string name;
	vector<Song> songs;

	// Inherited metadata (from the songs in the album)
	vector<string> genres;
	pair<int, int> year_range;

	Album();
	~Album();
	Album(string name);
	void inherit_metadata();
	void print();
};
