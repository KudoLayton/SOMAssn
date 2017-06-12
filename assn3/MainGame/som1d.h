#pragma once
#include "som.h"

class SOM1D : public SOM {
public:
	SOM1D(int x, int n, int out, float T1, float T2, float nei, float sig);
	virtual float learn(float *trainData, int t);
	virtual float learnData(Data& data, int t);
	virtual void drawMap(Data& data);

private:
	int n;
};

