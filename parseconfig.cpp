#include "parseconfig.h"
#include <cstring>
#include <string>
#include <vector>

vector<configData> ParseConfig::readConfig(string configFile) {
	vector<configData> data;
	char* str = new char[256];
	ifstream file(configFile);
	while (!file.eof())
	{
		file.getline(str, 256);
		char* pch = strtok(str, " ");
		int flag = 0, maxFlag = 5;
		configData cnf;
		while (pch != NULL)
		{
			if (flag == 0) {
				string s = string(pch);
				cnf.nameFilter = s;
			}
			if (flag != 0) {
				int ch = (*pch) - '0';
				cnf.coordinates.push_back(ch);
			}
			flag++;
			if (flag == maxFlag) {
				flag = 0;
				data.push_back(cnf);
				//cnf.nameFilter = NULL;
				cnf.coordinates.clear();
			}
			pch = strtok(NULL, " ");
		}
	}
	file.close();
	

	return data;
}