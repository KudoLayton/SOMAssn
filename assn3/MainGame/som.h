#pragma once
#include <vector>

class SOM {
public:
	SOM();
	SOM(int x, int y, float T1, float T2, float nei);
	~SOM();
	virtual void drawMap() = 0;
	virtual int learn(std::vector<float> &trainData) = 0;
	virtual int process(std::vector<float> &inputData);

protected:
	float T1, T2, nei;
	std::vector<float> output;
	std::vector<std::vector<float> *> weight;
	int learn(std::vector<float> &trainData, int dim);
	float learning(int i, int winner, int dim);
};