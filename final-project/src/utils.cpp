#include <algorithm>

#include "utils.h"

using std::transform;

string to_lowercase(string text) {
	// Copy the string so our operations don't mutate the original
	string copy;
	copy.assign(text);

	transform(copy.begin(), copy.end(), copy.begin(), ::tolower);

	return copy;
}


vector<string> string_split(string text, char splitter) {
	vector<int> indices = string_get_indices_of(text, splitter);
	vector<string> split;

	// Special case: before the first comma
	split.push_back(text.substr(0, indices[0]));

	// General case: between the first and last commas
	for (int i = 1, n = indices.size(); i < n; i++) {
		split.push_back(text.substr(indices[i - 1] + 1, indices[i] - indices[i - 1] - 1));
	}

	// Special case: after the last comma
	split.push_back(text.substr(indices.back() + 1, text.length()));

	return split;
}


vector<int> string_get_indices_of(string text, char splitter) {
	vector<int> indices;

	for (int i = 0, n = text.length(); i < n; i++) {
		if (text[i] == splitter) indices.push_back(i);
	}

	return indices;
}

bool string_contains(string text, char character) {
	// Return true when the element is found
	for (char text_character : text) {
		if (text_character == character) return true;
	}

	// Return false otherwise
	return false;
}