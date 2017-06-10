#include <fstream>
#include "traindata.h"

Data::somData::somData(float *input, int label): input(input), label(label) {
	
}

Data::somData::~somData() {
	delete input;
}

Data::Data(const char* inputFile, const char* labelFile, int inputNum, int labelNum) {
	std::fstream iStream(inputFile, std::ios_base::in);
	std::fstream lStream(labelFile, std::ios_base::in);
	int cnt = 0;
	char buf[BUFSIZ];
	iStream.getline(buf, BUFSIZ);
	lStream.getline(buf, BUFSIZ);
	while (!iStream.eof()) {
		float *newInput = new float[inputNum];
		int newLabel, labelValue;
		for (int i = labelNum - 1; i >= 0; i--) {
			lStream >> newLabel;
			if (newLabel) {
				labelValue = i;
			}
		}
		for (int i = 0; i < inputNum; i++) {
			iStream >> newInput[i];
		}
		somData *newData = new somData(newInput, labelValue);
		switch (cnt % 3) {
		case 0:
			trainData.push_back(newData);
			break;
		case 1:
			valiData.push_back(newData);
			break;
		case 2:
			testData.push_back(newData);
			break;
		}
	}
}

Data::~Data() {
	for (unsigned int i = 0; i < trainData.size(); i++) {
		delete trainData[i];
	}
	for (unsigned int i = 0; i < valiData.size(); i++) {
		delete valiData[i];
	}
	for (unsigned int i = 0; i < testData.size(); i++) {
		delete testData[i];
	}
}