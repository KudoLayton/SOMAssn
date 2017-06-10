#pragma once
#include <vector>

class SOM {
public:
	SOM();
	SOM(int x, int y, int out, float T1, float T2, float nei, float sig);
	~SOM();
	void resetLabel();
	void Labeling(std::vector<float> &trainData, std::vector<int> Label);
	virtual void drawMap() = 0;
	virtual int learn(std::vector<float> &trainData, int t) = 0;
	void process(std::vector<float> &inputData);
	
protected:
	int out;
	float T1, T2, nei, sig;
	std::vector<int> output;
	std::vector<std::vector<float> *> weight;
	std::vector<std::vector<int> *> label;
	int subLearn(std::vector<float> &trainData, int dim, int t);
	float learning(int i, int winner, int dim, int t);
};