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

void SOM2D::drawMap() {
	for (unsigned int i = 0; i < label.size(); i++) {
		COLOR col;
		int* thisLabel = label[i];
		col = RGB(thisLabel[0] * 255 / total, thisLabel[1] * 255 / total, thisLabel[2] * 255 / total);
		col += 0xFF << 24;
		draw_charc(' ', i % n, i / n, col, col);
	}
}

int SOM2D::learn(float *trainData, int t) {
	return subLearn(trainData, n, t);
}
void SOM2D::learnData(Data& data, int t) {
	int totalTrain = data.trainData.size();
	std::vector<Data::somData *> &inputData = data.trainData;
	for (int i = 0; i < totalTrain; i++) {
		subLearn(inputData[i]->input, n, t);
	}
}
