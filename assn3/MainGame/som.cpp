#include <vector>
#include <cmath>
#include "som.h"

SOM::SOM() {

}

SOM::SOM(int x, int y, int out, float T1, float T2, float nei, float sig): out(out), T1(T1), T2(T2), nei(nei), sig(sig) {
	for (int i = 0; i < y; i++) {
		std::vector<float> *newWeight = new std::vector<float>;
		std::vector<float> *newOut = new std::vector<float>;
		std::vector<float>& thisWeight = *newWeight;
		std::vector<float>& thisOut = *newOut;
		for (int j = 0; j < x; j++) {
			thisWeight.push_back((float)std::rand() / RAND_MAX);
		}

		for (int j = 0; j < out; j++) {
			thisOut.push_back(0);
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


int SOM::subLearn(std::vector<float>& trainData, int dim, int t) {
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
			thisWeight[j] = thisWeight[j] + nei * expf(- t / T2) * learning(i, winner, dim, t) * (trainData[j] - thisWeight[j]);
		}
	}
	return winner;
}


void SOM::process(std::vector<float> &inputData) {
	const int y = weight.size();
	const int x = weight[0]->size();
	
	for (int i = 0; i < y; i++) {
		float sum = 0;
		std::vector<float> &thisWeight = *weight[i];
		for (int j = 0; j < x; j++) {
			sum += inputData[j] * thisWeight[j];
		}
		output[i] = sum > 0.5 ? 1 : 0;
	}
}

float SOM::learning(int i, int winner, int dim, int t) {
	float sigma = sig * std::expf(- t / T1);
	float d;

	switch (dim) {
	case 1:
		d = (float)i - winner;
		d *= d;
		break;
	default:
		float temp = (float)(i / dim - winner / dim);
		d = temp * temp;
		temp = (float)(i % dim - winner % dim);
		d += temp * temp;
		break;
	}
	return expf(-d / (2 * sigma * sigma));
}

void SOM::resetLabel() {
	for (unsigned int i = 0; i < label.size(); i++) {
		std::vector<int> & thisLabel = *label[i];
		for (unsigned int j = 0; j < thisLabel.size(); j++) {
			thisLabel[i] = 0;
		}
	}
}

void SOM::Labeling(std::vector<float> &trainData, std::vector<int> Label) {
	process(trainData);
	for (unsigned int i = 0; i < output.size(); i++) {
		if (output[i]) {
			label[i]->operator[](Label[i])++;
		}
	}
}
