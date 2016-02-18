#pragma once

#include <vector>
#include <string>

using namespace std;

namespace strtool
{
	string trim(const string& str);
	int split(const string& str, vector<string>& ret_, string sep = ",");
}