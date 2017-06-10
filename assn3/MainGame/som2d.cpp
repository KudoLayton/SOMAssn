#ifndef JOKEBOX_LEVEL_A
#define JOKEBOX_LEVEL_A	
#include "jokebox.h"
#endif
#include <cmath>
#include "som2d.h"

SOM2D *SOM2D::globalSOM2D = NULL;

SOM2D::SOM2D(int x, int n, int out, float T1, float T2, float nei, float sig) : SOM(x, n * n, out, T1, T2, nei, sig), n(n) {
	
}

void SOM2D::drawMap() {
	for (unsigned int i = 0; i < label.size(); i++) {
		unsigned int col;
		int total;
		int* thisLabel = label[i];
		total = thisLabel[0] + thisLabel[1] + thisLabel[2];
		col = RGB(thisLabel[0] * 255 / total, thisLabel[1] * 255 / total, thisLabel[2] * 255 / total);
		draw_charc(' ', i % n, i / n, col, col);
	}
}

int SOM2D::learn(float *trainData, int t) {
	return subLearn(trainData, n, t);
}
