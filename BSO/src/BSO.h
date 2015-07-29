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
#include "SharedMemoryEvaluator.h"

struct IMParams
{
	//Total 50 bits
	int TankValue;     //5 bits
	int TankRadius;    //4 bits
	int MarineValue;   //5 bits
	int MarineRadius;  //4 bits
	int A;             //6 bits
	int B;             //6 bits
	int C;             //6 bits
	int l;             //4 bits
	int m;             //4 bits
	int n;             //4 bits
};

class BSO
{
private:
	static const int chromoLength = 48;    //set the init chromosome length
	static const int imLength = 18;
	static const int pfLength = 30;
	int type;   // 0 - all chromosome are random, 1 - im random, 2 - pf random
	std::bitset<chromoLength> chromosome;
	void init2seed();
	IMParams getInitialIMParams();
	int decode(int param[], int length);

public:
	BSO(int type, int seed);
	~BSO();

	double evaluate(std::bitset<chromoLength> params);

	void run();

	std::string toString(IMParams* params);
	void record2file();
	void log2file(int evaluateCount);
	
protected:
	int score;
	int latestScore;
	IMParams currentParams;
	IMParams adjustedParams;
	void randomFlip(std::bitset<chromoLength> &params, int type);
	void flip(std::bitset<chromoLength> & params, int pos);
	IMParams decode(std::bitset<chromoLength> codes);
	void encode(IMParams params, std::bitset<chromoLength> &chromo);
	
	bool readfile();
	int seed;
	int maxCount;
	SharedMemoryEvaluator* sharedMemoryEvaluator;
};