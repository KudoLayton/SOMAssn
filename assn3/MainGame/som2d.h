#pragma once
#include "som.h"

class SOM2D:public SOM {
public:
	SOM2D(int x, int n, int out, float T1, float T2, float nei, float sig);
	static SOM2D *globalSOM2D;
	virtual int learn(float *trainData, int t);
	virtual void drawMap();

private:
	int n;
};