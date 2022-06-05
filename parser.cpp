#include "parser.h"
#include <iostream>
#include <fstream>
#include <string>

vector<vector<double>> parse(string fileName) {
	vector<vector<double>> fullVec;
	vector<double> nestedVec;

	ifstream infile(fileName);
	string line;

	if (infile.is_open()) {
		cout << "Parsing data...\n";
		while (getline(infile, line)) {
			while (line != "\0") {
				line = line.substr(line.find_first_not_of(" ")); // Trim beginning spaces
				string temp = line.substr(0, line.find(" ")); // Get number
				nestedVec.push_back(stod(temp)); // Put number into vector
				if (line.find(" ") != string::npos) {
					line = line.substr(line.find(" ")); // Trim spaces again
				}
				else {
					break; // End of line
				}
			}
			fullVec.push_back(nestedVec);
			nestedVec.clear();
		}
		infile.close();
	}
	else {
		cout << "Failed to open\n";
	}

	/*for (int i = 0; i < fullVec.size(); ++i) {
		for (int j = 0; j < fullVec[i].size(); ++j) {
			cout << fullVec[i][j] << " ";
		}
		cout << endl;
	}*/


	return fullVec;
}