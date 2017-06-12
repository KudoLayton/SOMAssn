#include <vector>
#include <cmath>
#include "som.h"
#include "traindata.h"
SOM* SOM::globalSOM = NULL;
SOM::SOM() {

}

SOM::SOM(int x, int y, int out, float T1, float T2, float nei, float sig):x(x), y(y), out(out), T1(T1), T2(T2), nei(nei), sig(sig) {
	for (int i = 0; i < y; i++) {
		float *newWeight = new float[x];
		int *newLabel = new int[out];

		for (int j = 0; j < x; j++) {
			newWeight[j] = (float)std::rand() / RAND_MAX;
		}
		
		for (int j = 0; j < out; j++) {
			newLabel[j] = 0;
		}
		weight.push_back(newWeight);
		output.push_back(0);
		label.push_back(newLabel);
	}
}

SOM::~SOM() {
	for (int i = 0; i < y; i++) {
		delete weight[i];
		delete label[i];
	}
}


float SOM::subLearn(float* trainData, int dim, int t) {
	float* d = new float[weight.size()];
	for (int i = 0; i < y; i++) {
		float sum = 0;
		float *thisWeight = weight[i];
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
	float winnerDistance = d[winner];
	delete d;

	//std::vector<float> &winnerWeight = *weight[winner];
	for (int i = 0; i < y; i++) {
		float *thisWeight = weight[i];
		float learningConst = nei * std::expf(-t / T2) * learning(i, winner, dim, t);
		for (int j = 0; j < x; j++) {
			thisWeight[j] += learningConst * (trainData[j] - thisWeight[j]);
		}
	}
	return winnerDistance;
}


void SOM::process(float *inputData) {
	float bestH;
	int bestHIndex;
	for (int i = 0; i < y; i++) {
		float sum = 0;
		float* thisWeight = weight[i];
		for (int j = 0; j < x; j++) {
			sum += (inputData[j] - thisWeight[j]) * (inputData[j] - thisWeight[j]);
		}
		output[i] = sum < 225 ? 1 : 0;
		if (i == 0)
			bestH = sum;
		else if (sum < bestH) {
			bestH = sum;
			bestHIndex = i;
		}
	}
	//output[bestHIndex] = 1;
}

float SOM::learning(int i, int winner, int dim, int t) {
	float sigma = sig * std::expf(-t / T1);
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
	total = 0;
	for (unsigned int i = 0; i < label.size(); i++) {
		int *thisLabel = label[i];
		for (int j = 0; j < out; j++) {
			thisLabel[j] = 0;
		}
	}
}

void SOM::Labeling(float *trainData, int thisLabel) {
	process(trainData);
	for (unsigned int i = 0; i < output.size(); i++) {
		if (output[i]) {
			label[i][thisLabel]++;
		}
	}
	total++;
}

int SOM::makeResult(float* inputData) {
	int* res = new int[out];
	int result;
	process(inputData);
	for (int i = 0; i < out; i++) {
		res[i] = 0;
	}
	for (int i = 0; i < y; i++) {
		if (output[i] == 1) {
			int *thisLabel = label[i];
			/*
			for (int j = 0; j < out; j++) {
				res[j] += thisLabel[j];
			}*/
			int voteLabel = 0;
			for (int j = 0; j < out; j++) {
				if (thisLabel[j] > thisLabel[voteLabel]) {
					voteLabel = j;
				}
			}
			if (voteLabel == 0) {
				if (thisLabel[0] > 0) {
					res[0]++;
				}
			}
			else {
				res[voteLabel]++;
			}
		}
	}
	result = 0;
	for (int i = 0; i < out; i++) {
		if (res[result] < res[i]) {
			result = i;
		}
	}
	delete res;
	return result;
}

float SOM::makeResultData(Data& data) {
	std::vector<Data::somData *>& list = data.testData;
	int total = list.size();
	int correct = 0;
	for (int i = 0; i < total; i++) {
		if (list[i]->label == makeResult(list[i]->input)) {
			correct++;
		}
	}
	return (float)correct / total * 100;
}
