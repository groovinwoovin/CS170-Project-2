#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <time.h>
#include <math.h>

using namespace std;

double leaveOneOutCrossValidation(vector<vector<double>> data, vector<int> featureSet);
void forwardSelection(vector<vector<double>> data);
//void backwardsElimination(vector<vector<double>> data);

vector<vector<double>> parse(string fileName);

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

	// Normalize dataset
	double mean;
	double stddev;

	vector<double> values;
	for (int i = 0; i < dataSet[0].size(); ++i) { // For each feature
		for (int j = 0; j < dataSet.size(); ++j) {
			// Get sum of all points

			
		}
		// Get average

		// 
	}

	// Run algorithm
	switch (userSelection) {
	case 1:
		forwardSelection(dataSet);
		break;
	case 2:
		//backwardsElimination(dataSet);
		break;
	}

	return 0;
}

double leaveOneOutCrossValidation(vector<vector<double>> data, vector<int> featureSet) {
	// Set inital things
	int accuracy = 0;
	int dataFeatureSize = data[0].size();

	// Go through each dataPoint
	for (int i = 0; i < data.size(); ++i) {
		// Leave one point out
		vector<double> leftOutPoint = data[i];

		double distance = DBL_MAX; // Set maximun distance
		double tempDistance = 0;
		int closestPointIndex = 0; // To find the location of the closest neighbor to left out point

		vector<double> inSetPoint; // Point to calculate distance from left out
		// Calculate distance from beginning to left out point
		for (int j = 0; j < i; ++j) {
			inSetPoint = data[j];
			// Subtract then square the difference between features
			for (int k = 0; k < featureSet.size(); ++k) {
				tempDistance += pow((leftOutPoint[featureSet[k]] - inSetPoint[featureSet[k]]), 2);
			}
			tempDistance = sqrt(tempDistance);

			// overWrite the closest neighbor if necessary
			if (tempDistance < distance) {
				distance = tempDistance;
				closestPointIndex = j;
			}
		}

		// Calculate distance from left out point + 1 to end
		for (int j = i + 1; j < data.size(); ++j) {
			inSetPoint = data[j];
			for (int k = 0; k < featureSet.size(); ++k) {
				tempDistance += pow((leftOutPoint[featureSet[k]] - inSetPoint[featureSet[k]]), 2);
			}
			tempDistance = sqrt(tempDistance);
			if (tempDistance < distance) {
				distance = tempDistance;
				closestPointIndex = j;
			}
		}

		// Check if closest Neighbor is the same as the datapoint
		if (((int) leftOutPoint[0]) == ((int) data[closestPointIndex][0])) {
			accuracy++;
		}
	}

	return ((double) accuracy/data.size());
}

void forwardSelection(vector<vector<double>> data) {
	// Initialize empty set
	vector<int> currentFeatureSet;

	int featureAmount = data[0].size() - 1;

	// Set default rate accuracy (most chosen point out of the rest)
	double highestAccuracy = 0.;
	int dataPoint1 = 0;
	int dataPoint2 = 0;
	for (int i = 0; i < data.size(); ++i) {
		if (data[i][0] == 1) {
			dataPoint1++;
		}
		else {
			dataPoint2++;
		}
	}
	highestAccuracy = (dataPoint1 > dataPoint2) ? (double) dataPoint1 / data.size() : (double) dataPoint2 / data.size();

	// Step through the different depths
	for (int i = 0; i < featureAmount; ++i) {
		// Set the potential new featureSet as the current Feature Set
		vector<int> potentialNewFeatureSet = currentFeatureSet;
		bool ishigher = false;
		double currentAccuracy = 0.;
		int highestValue;
		cout << "On depth " << i + 1 << endl;

		for (int k = 1; k < featureAmount + 1; ++k) {
			// Check if the current feature is not in the set
			if (find(potentialNewFeatureSet.begin(), potentialNewFeatureSet.end(), k) == potentialNewFeatureSet.end()) {
				//cout << "Considering adding feature " << data[k] << endl;
				potentialNewFeatureSet.push_back(k);

				cout << "Testing featureSet {";
				for (int l = 0; l < potentialNewFeatureSet.size(); ++l) {
					cout << potentialNewFeatureSet[l];
					(l < potentialNewFeatureSet.size() - 1) ? cout << ", " : cout << "";
				}
				cout << "}";
				// Add the feature to the set, and calculate the set
				currentAccuracy = leaveOneOutCrossValidation(data, potentialNewFeatureSet);
				cout << " with accuracy of: " << currentAccuracy << endl;

				potentialNewFeatureSet.pop_back();
				
				// If the set is greater than the accuracy of the currentFeatureSet, overwrite the currentFeature Set and highest Accuracy and toggle ishigher bool
				if (currentAccuracy >= highestAccuracy) {
					highestAccuracy = currentAccuracy;
					highestValue = k;
					ishigher = true;
				}
			}
		}
		
		// If any of the subsets is unable to give a larger accuracy than the current set, break and let user know that accuracy dropped
		if (!ishigher) {
			cout << "Subset accuracy dropped, ending search\n";
			break;
		}
		else {
			//cout << "Highest value was " << highestValue << " with accuracy of " << highestAccuracy << endl;
			currentFeatureSet.push_back(highestValue);
			cout << "Subset with highest accuracy was: {";
			for (int i = 0; i < currentFeatureSet.size(); ++i) {
				cout << currentFeatureSet[i];
				(i < currentFeatureSet.size() - 1) ? cout << ", " : cout << "";
			}
			cout << "}\n";
			cout << "Accuracy of: " << highestAccuracy << endl;
		}
	}

	cout << "Subset with highest accuracy is: {";
	for (int i = 0; i < currentFeatureSet.size(); ++i) {
		cout << currentFeatureSet[i];
		(i < currentFeatureSet.size() - 1) ? cout << ", " : cout << "";
	}
	cout << "}\n";
	cout << "Accuracy of: " << highestAccuracy << endl;
}

//void backwardsElimination(vector<double> data) {
//	// Initalize full set
//	vector<double> currentFeatureSet;
//	double highestAccuracy = 0.;
//
//	currentFeatureSet = data;
//
//	// Step through the different depths
//	for (int i = 0; i < data.size(); ++i) {
//		// Set potential set as CurrentFeatureSet
//		vector<double> potentialNewFeatureSet = currentFeatureSet;
//		bool ishigher = false;
//		double currentAccuracy = 0.;
//		double highestValue;
//
//		for (int j = 0; j < potentialNewFeatureSet.size(); ++j) {
//			// Create subset without a feature
//			vector<double> subset;
//			for (int k = 0; k < j; ++k) {
//				subset.push_back(potentialNewFeatureSet[k]);
//			}
//			for (int l = j + 1; l < potentialNewFeatureSet.size(); ++l) {
//				subset.push_back(potentialNewFeatureSet[l]);
//			}
//
//			// Calculate the set
//			//currentAccuracy = leaveOneOutCrossValidation(subset);
//			
//			cout << "Calculated accuracy for {";
//			for (int i = 0; i < subset.size(); ++i) {
//				cout << subset[i];
//				(i < subset.size() - 1) ? cout << ", " : cout << "";
//			}
//			cout << "} " << currentAccuracy << " "<< endl;
//
//			// If the set is greater than the accuracy of the currentFeatureSet, overwrite the currentFeature Set and highest Accuracy and toggle ishigher bool
//			if (currentAccuracy > highestAccuracy) {
//				highestAccuracy = currentAccuracy;
//				currentFeatureSet = subset;
//				ishigher = true;
//			}
//		}
//
//		if (!ishigher) {
//			cout << "Subset accuracy dropped, ending search\n";
//			break;
//		}
//	}
//
//	cout << "Subset with highest accuracy is: {";
//	for (int i = 0; i < currentFeatureSet.size(); ++i) {
//		cout << currentFeatureSet[i];
//		(i < currentFeatureSet.size() - 1) ? cout << ", " : cout << "";
//	}
//	cout << "}\n";
//}

vector<vector<double>> parse(string fileName) {
	vector<vector<double>> fullVec;
	vector<double> nestedVec;

	ifstream infile (fileName);
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