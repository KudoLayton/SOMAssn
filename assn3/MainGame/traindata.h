#pragma once
#include <vector>
class Data {
public:
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
	std::vector<somData *> valiData;
	std::vector<somData *> testData;
private:
		
};