#include "sortingFunctions.h"
#include <vector>
#include <iostream>

using namespace std;

double leaveOneOutCrossValidation(vector<vector<double>> data, vector<int> featureSet) {
	// Set initial things
	int accuracy = 0;
	int dataFeatureSize = data[0].size();

	// Go through each dataPoint
	for (int i = 0; i < data.size(); ++i) {
		// Leave one point out
		vector<double> leftOutPoint = data[i];

		double distance = DBL_MAX; // Set maximum distance
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
		if (((int)leftOutPoint[0]) == ((int)data[closestPointIndex][0])) {
			accuracy++;
		}
	}

	return ((double)accuracy / data.size());
}

void forwardSelection(vector<vector<double>> data) {
	// Initialize empty set
	vector<int> currentFeatureSet;

	int amountOfFeatures = data[0].size() - 1;

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
	highestAccuracy = (dataPoint1 > dataPoint2) ? (double)dataPoint1 / data.size() : (double)dataPoint2 / data.size();

	// Step through the different depths
	for (int i = 0; i < amountOfFeatures; ++i) {
		// Set the potential new featureSet as the current Feature Set
		vector<int> potentialNewFeatureSet = currentFeatureSet;
		bool ishigher = false;
		double currentAccuracy = 0.;
		int highestValueIndex;
		cout << "On depth " << i + 1 << endl;

		for (int k = 1; k < amountOfFeatures + 1; ++k) {
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
					highestValueIndex = k;
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
			currentFeatureSet.push_back(highestValueIndex);
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

void backwardsElimination(vector<vector<double>> data) {
	// Initialize full set
	vector<int> currentFeatureSet;
	int amountOfFeatures = data[0].size() - 1;

	for (int i = 0; i < amountOfFeatures; ++i) {
		currentFeatureSet.push_back(i + 1);
	}

	// Set default rate accuracy

	double highestAccuracy = 0.;

	cout << "Testing featureSet {";
	for (int l = 0; l < currentFeatureSet.size(); ++l) {
		cout << currentFeatureSet[l];
		(l < currentFeatureSet.size() - 1) ? cout << ", " : cout << "";
	}
	cout << "}";

	highestAccuracy = leaveOneOutCrossValidation(data, currentFeatureSet);
	cout << " with accuracy of: " << highestAccuracy << endl;

	// Step through the different depths
	for (int i = 0; i < amountOfFeatures; ++i) {
		// Set potential set as CurrentFeatureSet
		vector<int> potentialNewFeatureSet = currentFeatureSet;
		bool ishigher = false;
		double currentAccuracy = 0.;
		double highestValueIndex;
		cout << "On depth " << i + 1 << endl;

		for (int j = 1; j < potentialNewFeatureSet.size(); ++j) {
			// Create subset without a feature
			vector<int> subset;
			for (int k = 0; k < j; ++k) {
				subset.push_back(potentialNewFeatureSet[k]);
			}
			for (int l = j + 1; l < potentialNewFeatureSet.size(); ++l) {
				subset.push_back(potentialNewFeatureSet[l]);
			}

			cout << "Testing featureSet {";
			for (int l = 0; l < potentialNewFeatureSet.size(); ++l) {
				cout << potentialNewFeatureSet[l];
				(l < potentialNewFeatureSet.size() - 1) ? cout << ", " : cout << "";
			}
			cout << "}";

			// Calculate the set
			currentAccuracy = leaveOneOutCrossValidation(data, subset);
			cout << " with accuracy of: " << currentAccuracy << endl;

			// If the set is greater than the accuracy of the currentFeatureSet, overwrite the currentFeature Set and highest Accuracy and toggle ishigher bool
			if (currentAccuracy >= highestAccuracy) {
				highestAccuracy = currentAccuracy;
				currentFeatureSet = subset;
				ishigher = true;
			}
		}

		if (!ishigher) {
			cout << "Subset accuracy dropped, ending search\n";
			break;
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