#pragma once

#include <string>
#include <vector>

using std::vector;
using std::string;

string csv_encode(vector<string> strings);
vector<string> csv_decode(string csv_string);
vector<int> get_comma_indices(string csv_string);