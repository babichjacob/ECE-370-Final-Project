#include "csv.h"


vector<int> get_comma_indices(string csv_string) {
	vector<int> indices;

	for (int i = 0, n = csv_string.length(); i < n; i++) {
		if (csv_string[i] == ',') indices.push_back(i);
	}

	return indices;
}


string csv_encode(vector<string> strings) {
	string result = "";

	int n = strings.size();

	for (int i = 0; i < n - 1; i++) {
		result += strings[i] + ",";
	}
	// Do not put a comma after the last one
	result += strings[n - 1];

	return result;
}


vector<string> csv_decode(string csv_string) {
	vector<int> indices = get_comma_indices(csv_string);
	vector<string> parsed;

	// Special case: before the first comma
	parsed.push_back(csv_string.substr(0, indices[0]));

	// General case: between the first and last commas
	for (int i = 1, n = indices.size(); i < n; i++) {
		parsed.push_back(csv_string.substr(indices[i - 1] + 1, indices[i] - indices[i - 1] - 1));
	}

	// Special case: after the last comma
	parsed.push_back(csv_string.substr(indices.back() + 1, csv_string.length()));

	return parsed;
}
