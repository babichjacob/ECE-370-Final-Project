#pragma once

#include <map>
#include <string>

#include "csv.h"


using std::map;
using std::string;

constexpr auto PREFERENCES_FILE = "bin/data/.MyTunes/preferences.csv";

// Global variable
// User preferences are just a key-value store of strings, stored in a CSV file

typedef map<string, string> Preferences;


Preferences preferences_init();
void preferences_set(Preferences preferences, string key, string value);
string preferences_get(Preferences preferences, string key, string default_value);