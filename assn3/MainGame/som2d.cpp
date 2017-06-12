#ifndef JOKEBOX_LEVEL_A
#define JOKEBOX_LEVEL_A	
#include "jokebox.h"
#endif
#include <cmath>
#include <iostream>
#include "som2d.h"
#include "traindata.h"

SOM2D::SOM2D(int x, int n, int out, float T1, float T2, float nei, float sig) : SOM(x, n * n, out, T1, T2, nei, sig), n(n) {
	SOM::globalSOM = this;
}

void SOM2D::drawMap(Data& data) {
	for (unsigned int i = 0; i < label.size(); i++) {
		COLOR col;
		int* thisLabel = label[i];
		int r = thisLabel[0] * 255 / (data.trainTotal[0]);
		int g = thisLabel[1] * 255 / (data.trainTotal[1]);
		int b = thisLabel[2] * 255 / (data.trainTotal[2]);
		int total = r + g + b;
		total = total == 0 ? 1 : total;
		col = RGB(r * 255 / total, g * 255 / total, b * 255 / total);
		col += 0xFF << 24;
		draw_charc(' ', i % n, i / n, col, col);
	}
}

float SOM2D::learn(float *trainData, int t) {
	return subLearn(trainData, n, t);
}

float SOM2D::learnData(Data& data, int t) {
	int totalTrain = data.trainData.size();
	float minDistance = 0;
	std::vector<Data::somData *> &inputData = data.trainData;
	minDistance = subLearn(inputData[0]->input, n, t);
	for (int i = 1; i < totalTrain; i++) {
		float thisDistance;
		thisDistance = subLearn(inputData[i]->input, n, t);
		if (thisDistance < minDistance) {
			minDistance = thisDistance;
		}
	}
	return minDistance;
}
