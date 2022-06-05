#ifndef __SORTINGFUNCTIONS_H__
#define __SORTINGFUNCTIONS_H__

#include <vector>

using namespace std;

double leaveOneOutCrossValidation(vector<vector<double>> data, vector<int> featureSet);

void forwardSelection(vector<vector<double>> data);
void backwardsElimination(vector<vector<double>> data);

#endif // !__SORTINGFUNCTIONS_H__
