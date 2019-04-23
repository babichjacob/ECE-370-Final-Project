#pragma once

#include <string>
#include <vector>

using std::string;
using std::vector;

// Must be defined in this header file or else there will be `unresolved external symbol` error(s)
template<class T>
bool vector_contains(vector<T> the_vector, T the_value) {
	// Return true when the element is found
	for (auto& element : the_vector) {
		if (element == the_value) return true;
	}

	// Return false otherwise
	return false;
}


string to_lowercase(string text);


vector<string> string_split(string text, char splitter);
vector<int> string_get_indices_of(string text, char splitter);

bool string_contains(string text, char character);