#pragma once

#include <map>
#include <bitset>
#include <algorithm>
#include <math.h>
#include <iostream>
#include <string>
#include <iostream>
#include <sstream>
#include <iostream>
#include <fstream>

class BinaryHillClimberAvg
{
public:
	//constructor/functions
	BinaryHillClimberAvg();
	~BinaryHillClimberAvg(){}

	IMParams getCurrentParams();
    IMParams* getAdjustedParams();
	void modify();
    void useAdjustedParam();
	void setScore(int score);
	void setTempScore(int s);
	std::string toString(IMParams* params);
	int getScore(){return this->score;}
	int getTempScore(){return this->tempScore;}
	void record2file();
	
protected:
	int score;
	int tempScore;
	IMParams currentParams;
	IMParams adjustedParams;
	std::bitset<32> randomFlip(std::bitset<32> params);
	std::bitset<32> encode(IMParams params);
	IMParams decode(std::bitset<32> codes);
	void updateDate2file();
	void log2file();
	bool readfile();
	int evaluateCount;
	int seed;
	void generateRandomParameter(int seed);
private:
	void init2seed();
	void init2average();


};