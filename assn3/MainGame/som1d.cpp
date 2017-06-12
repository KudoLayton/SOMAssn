#ifndef JOKEBOX_LEVEL_A
#define JOKEBOX_LEVEL_A
#define JOKEBOX_LEVEL_B
#include "jokebox.h"
#endif
#include <cmath>
#include <iostream>
#include "som1d.h"
#include "traindata.h"

SOM1D::SOM1D(int x, int n, int out, float T1, float T2, float nei, float sig) : SOM(x, n, out, T1, T2, nei, sig), n(n) {
	SOM::globalSOM = this;
}

void SOM1D::drawMap(Data& data) {
	for (unsigned int i = 0; i < label.size(); i++) {
		
	}
}

float SOM1D::learn(float *trainData, int t) {
	return subLearn(trainData, 1, t);
}

float SOM1D::learnData(Data& data, int t) {
	int totalTrain = data.trainData.size();
	float minDistance = 0;
	std::vector<Data::somData *> &inputData = data.trainData;
	minDistance = subLearn(inputData[0]->input, 1, t);
	for (int i = 1; i < totalTrain; i++) {
		float thisDistance;
		thisDistance = subLearn(inputData[i]->input, 1, t);
		if (thisDistance < minDistance) {
			minDistance = thisDistance;
		}
	}
	return minDistance;
}
