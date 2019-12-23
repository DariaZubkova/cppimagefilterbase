#pragma once
#include <vector>
#include <cstring>
#include <string>
#include <fstream>
using namespace std;

typedef struct configData {
	string nameFilter;
	vector<int> coordinates;
}configData;

class ParseConfig {
public:
	vector<configData> readConfig(string configFile);
};