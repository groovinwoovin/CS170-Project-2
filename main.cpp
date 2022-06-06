#include <iostream>
#include <vector>
#include <string>
#include "sortingFunctions.h"
#include "parser.h"

using namespace std;


int main(){
	int featureNum;
	int userSelection;

	// Get featureSet to run from user
	cout << "Welcome to Woovin's Feature Selection Algorithm\n";
	cout << "Select the file to run:\n";
	cout << "1) Large Data Set\n";
	cout << "2) Small Data Set\n";
	cout << "3) Personal Large Data Set\n";
	cout << "4) Personal Small Data Set\n";
	cin >> featureNum;

	// Get user search selection
	cout << "Type the number of the algorithm you want to run\n"
		<< "1) Forward Selection\n"
		<< "2) Backward Elimination\n"
		<< "X) Woovin's Special Algorithm (Not implemented)\n";
	cin >> userSelection;

	// Parse the data
	vector<vector<double>> dataSet;
	switch (featureNum) {
	case 1:
		dataSet = parse("Large-test-dataset.txt");
		break;
	case 2:
		dataSet = parse("small-test-dataset.txt");
		break;
	case 3:
		dataSet = parse("CS170_Spring_2022_Large_data__106.txt");
		break;
	case 4:
		dataSet = parse("CS170_Spring_2022_Small_data__106.txt");
		break;
	default:
		cout << "Incorrect dataset";
	}

	//Debug, uncomment to see original dataset
	//cout << "BEFORE";
	//for (int i = 0; i < dataSet.size(); ++i) {
	//	for (int j = 0; j < dataSet[i].size(); ++j) {
	//		cout << dataSet[i][j] << " ";
	//	}
	//	cout << endl;
	//}

	// Normalize dataset
	double mean;
	double stddev;

	double sum = 0;

	cout << "Normalizing data...\n";
	vector<double> values;
	for (int i = 1; i < dataSet[0].size(); ++i) { // For each feature
		for (int j = 0; j < dataSet.size(); ++j) {
			// Get sum of all points
			sum += dataSet[j][i];
		}
		// Get average
		mean = sum / dataSet.size();

		sum = 0;
		// Get std dev (sqrt((sum(value - mean)^2)/(n - 1)))
		for (int k = 0; k < dataSet.size(); ++k) {
			sum = pow((dataSet[k][i] - mean), 2);
		}
		stddev = sqrt(sum / (dataSet.size() - 1));

		// Apply normalization to each point
		for (int l = 0; l < dataSet.size(); ++l) {
			dataSet[l][i] = (dataSet[l][i] - mean) / stddev;
		}
	}

	cout << "Data has " << dataSet[0].size() - 1 << " features and " << dataSet.size() << " instances\n";

	//Debug, uncomment to see normalized dataset
	/*cout << "AFTER\n";
	for (int i = 0; i < dataSet.size(); ++i) {
		for (int j = 0; j < dataSet[i].size(); ++j) {
			cout << dataSet[i][j] << " ";
		}
		cout << endl;
	}*/

	// Run algorithm
	switch (userSelection) {
	case 1:
		forwardSelection(dataSet);
		break;
	case 2:
		backwardsElimination(dataSet);
		break;
	}

	return 0;
}

