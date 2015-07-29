#pragma once

#include <windows.h>

#include <BWAPI.h>
#include <math.h>
#include <sstream>
#include <iostream>
#include <fstream>

#include "IMParams.h"
#include <tchar.h>

#define BUF_SIZE 256

class GA
{
public:
	static GA* getInstance();
	GA();
	~GA();

	int writeFitness2SharedMemory(int score);
	IMParams getParams();
	IMParams getParamsRange();
	void setScore(int score);
	int getScore(){return this->score;}
	void log2file(int score);

	
protected:
	int score;
	IMParams currentParams;
	IMParams currentParams_range;

	bool readfile();
	int evaluateCount;
private:
	int decode(int param[], int length);

	int initSharedMemory();
	void testInit(int buffer[]);
	void readParameterFromSharedMemory();

	void parseParameterInput(int buffer[], int length);

	HANDLE hMapFile;
	int* pBuf;

	int chromoLen;
	int signalLen;
    static const TCHAR szName[];
	static const char datafile[];
	static const char logfile[];
	static const char conffile[];
	
	bool initialized;

	static GA* instance;

	void readParams();
};