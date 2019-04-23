#include <algorithm>

#include "search_engine.h"
#include "utils.h"

using std::cout;
using std::endl;
using std::to_string;
using std::transform;


void search_index(SearchEngine* se, Song* song) {
	cout << "search_index: indexing " << song->title << endl;

	// Index each searchable property of the song
	for (string property : { song->title, song->artist, song->album_artist, song->album, song->genre, to_string(song->year) }) {
		search_connect_string_to_song(se, song, property);
	}
}

void search_connect_string_to_song(SearchEngine* se, Song* song, string text) {
	string lowercased = to_lowercase(text);

	// Recursively call this function on the words in the string too
	if (string_contains(text, ' ')) {
		for (auto word : string_split(text, ' ')) {
			search_connect_string_to_song(se, song, word);
		}
	}

	// Incremental index
	for (int i = 1, n = text.size() + 1; i < n; i++) {
		string substring = lowercased.substr(0, i);

		// No duplication
		if (!vector_contains((*se)[substring], song)) {
			se->at(substring).push_back(song);
		}
	}
}

vector<Song*> search_for(SearchEngine* se, string text) {
	// The first results in the list are the most relevant; the ones at the front the least so
	vector<vector<Song*>*> nested_results;
	
	cout << endl << "searching for " << text << endl;

	// Incremental search
	string lowercased = to_lowercase(text);

	for (int i = 1, n = lowercased.size() + 1; i < n; i++) {
		string substring = lowercased.substr(0, i);

		nested_results.push_back(&(*se)[substring]);

		cout << "search_for: checking substring " << substring << endl;
	}

	// Assemble a unique list of results
	vector<Song*> results;

	// Flip the order so that the first results are the most relevant
	for (auto it = nested_results.rbegin(); it != nested_results.rend(); it++) {
		for (auto sub_it = (*it)->rbegin(); sub_it != (*it)->rend(); sub_it++) {
			if (!vector_contains(results, *sub_it)) results.push_back(*sub_it);

			cout << "search_for: " << (*sub_it)->title << endl;
		}
	}

	return results;
}