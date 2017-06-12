#pragma once
#include <vector>

class Data;

class SOM {
public:
	SOM();
	SOM(int x, int y, int out, float T1, float T2, float nei, float sig);
	~SOM();
	static SOM* globalSOM;
	void resetLabel();
	float makeResultData(Data& data);
	void Labeling(float *trainData, int thisLabel);
	virtual void drawMap(Data& data) = 0;
	virtual float learn(float *trainData, int t) = 0;
	virtual float learnData(Data& data, int t) = 0;
	void process(float *inputData);
	int makeResult(float *inputData);
	
protected:
	int x, y, out, total;
	float T1, T2, nei, sig;
	std::vector<int> output;
	std::vector<float *> weight;
	std::vector<int *> label;
	float subLearn(float *trainData, int dim, int t);
	float learning(int i, int winner, int dim, int t);
};