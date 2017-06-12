#pragma once
#include <vector>
class Data {
public:
	static Data* globalData;
	int inputNum;
	int labelNum;
	class somData {
	public:
		somData(float *input, int label);
		~somData();
		float* input;
		int label;
	};
	Data(const char* inputFile, const char* labelFile, int inputNum, int labelNum);
	~Data();
	std::vector<somData *> trainData;
	std::vector<int> trainTotal;
	std::vector<somData *> valiData;
	std::vector<int> valiTotal;
	std::vector<somData *> testData;
	std::vector<int> testTotal;
};