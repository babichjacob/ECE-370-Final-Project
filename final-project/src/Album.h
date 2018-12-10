#include "Song.h"

using std::cout;
using std::endl;

class Album
{
public:
	string name;
	vector<Song> songs;

	Album();
	~Album();
	Album(string name);
};
