#pragma once
#include <vector>

class SOM {
public:
	SOM();
	SOM(int x, int y, int out, float T1, float T2, float nei, float sig);
	~SOM();
	void resetLabel();
	void Labeling(float *trainData, int thisLabel);
	virtual void drawMap() = 0;
	virtual int learn(float *trainData, int t) = 0;
	void process(float *inputData);
	
protected:
	int x, y, out;
	float T1, T2, nei, sig;
	std::vector<int> output;
	std::vector<float *> weight;
	std::vector<int *> label;
	int subLearn(float *trainData, int dim, int t);
	float learning(int i, int winner, int dim, int t);
};