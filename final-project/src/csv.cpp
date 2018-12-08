#include "csv.h"

vector<string> parse_csv_string_to_vector(string csv_string) {
	vector<int> indices = get_comma_indices(csv_string);
	vector<string> parsed;

	parsed.push_back(csv_string.substr(0, indices[0]));

	for (int i = 1, n = indices.size(); i < n; i++) {
		parsed.push_back(csv_string.substr(indices[i - 1] + 1, indices[i] - indices[i - 1] - 1));
	}

	parsed.push_back(csv_string.substr(indices.back() + 1, csv_string.length()));

	return parsed;
}


vector<int> get_comma_indices(string csv_string) {
	vector<int> indices;

	for (int i = 0, n = csv_string.length(); i < n; i++) {
		if (csv_string[i] == ',') indices.push_back(i);
	}

	return indices;
}