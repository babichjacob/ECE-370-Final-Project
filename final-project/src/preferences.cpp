#include "preferences.h"

#include <fstream>
#include <iostream>
#include <string>
#include <utility>
#include <vector>

using std::cout;
using std::endl;
using std::getline;
using std::ifstream;
using std::ofstream;
using std::pair;
using std::vector;



Preferences preferences_init() {
	Preferences preferences;

	ifstream file;
	file.open(PREFERENCES_FILE);

	string line;
	while (getline(file, line)) {
		vector<string> parsed = csv_decode(line);

		// Add the key (first item of the CSV line) and value (second item) to the map
		preferences[parsed[0]] = parsed[1];
	}

	file.close();

	return preferences;
}


void preferences_set(Preferences preferences, string key, string value) {
	preferences[key] = value;

	// Write the update to the preferences file
	ofstream file;
	file.open(PREFERENCES_FILE);

	for (auto key_value : preferences) {
		file << csv_encode({ key_value.first, key_value.second }) << endl;
	}

	file.close();
}

string preferences_get(Preferences preferences, string key, string default_value) {
	// Return the value if the preferences map contains it
	if (preferences.count(key)) {
		return preferences[key];
	}

	// Or else return the default value
	return default_value;
}
