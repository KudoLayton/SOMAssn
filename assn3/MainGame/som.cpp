#include <vector>
#include <cmath>
#include "som.h"

SOM::SOM() {

}

SOM::SOM(int x, int y, float T1, float T2, float nei): T1(T1), T2(T2), nei(nei) {
	for (int i = 0; i < y; i++) {
		std::vector<float> *newWeight = new std::vector<float>;
		std::vector<float>& thisWeight = *newWeight;
		for (int j = 0; j < x; j++) {
			thisWeight.push_back(std::rand() / RAND_MAX);
		}
		weight.push_back(newWeight);
	}
}

SOM::~SOM() {
	int y = weight.size();

	for (int i = 0; i < y; i++) {
		delete weight[i];
	}
}


int SOM::learn(std::vector<float>& trainData, int dim) {
	float* d = new float[weight.size()];
	int y = weight.size();
	int x = weight[0]->size();
	for (int i = 0; i < y; i++) {
		float sum = 0;
		std::vector<float> &thisWeight = *weight[i];
		for (int j = 0; j < x; j++) {
			sum += powf(thisWeight[j] - trainData[j], 2);
		}
		d[i] = sum;
	}
	
	int winner = 0;
	
	for (int i = 0; i < y; i++) {
		if (d[i] < d[winner]) {
			winner = i;
		}
	}
	delete d;

	//std::vector<float> &winnerWeight = *weight[winner];
	for (int i = 0; i < y; i++) {
		std::vector<float> &thisWeight = *weight[i];
		for (int j = 0; j < x; j++) {
			thisWeight[j] = thisWeight[j] + learning(i, winner, dim) * (trainData[j] - thisWeight[j]);
		}
	}
	return winner;
}


int SOM::process(std::vector<float> &inputData) {
	const int y = weight.size();
	const int x = weight[0]->size();
	int maxIndex;
	float max;
	
	for (int i = 0; i < y; i++) {
		float sum = 0;
		std::vector<float> &thisWeight = *weight[i];
		for (int j = 0; j < x; j++) {
			sum += inputData[j] * thisWeight[j];
		}
		output[i] = sum;
	}

	maxIndex = 0;
	max = output[0];
	for (int i = 0; i < y; i++) {
		if (max < output[i]) {
			maxIndex = i;
		}
	}
	return maxIndex;
}

float SOM::learning(int i, int winner, int dim) {

}
