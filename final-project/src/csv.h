#pragma once

#include <string>
#include <vector>

using std::vector;
using std::string;

vector<string> parse_csv_string_to_vector(string csv_string);
vector<int> get_comma_indices(string csv_string);