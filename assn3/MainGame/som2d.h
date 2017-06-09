#pragma once
#include "som.h"

class SOM2D:public SOM {
public:
	SOM2D(int x, int n, float T1, float T2, float nei);
	virtual int learn(std::vector<float> &trainData);
	virtual void drawMap();

private:
	int n;
};