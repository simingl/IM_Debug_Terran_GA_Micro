#pragma once

#include "BinaryHillclimber.h"
#include <string>

using namespace std;

BinaryHillClimber::BinaryHillClimber(int type, int seed)
{
	sharedMemoryEvaluator = new SharedMemoryEvaluator();
	this->type = type;
	this->seed = seed; //default is 2
	init2seed();
}

void BinaryHillClimber::init2seed()
{
	maxCount = 4000;

	srand(seed); //set rand seed

	IMParams param = this->getInitialIMParams();
	this->encode(param, chromosome); //initialize chromosome

	/*
	//randomly genereate first chromosome
	for(int i=0;i< imLength; i++)
	{
		if(this->type != 2)  // type = 0,1 flip im, type = 2 fix im
		{
			this->chromosome[i] = rand()%2;
		}
	}

	//fix pf value
	for(int i=imLength; i< chromoLength; i++)
	{
		if(this->type != 1) // type = 0,2 flip pf, type = 1 fix pf
		{
			this->chromosome[i] = rand()%2;
		}
	}
	*/

	//TEST:set to 10,8,10,8,   10*100, 30*10000, 25, 10, 10, 4
	//this->chromosome = {0,1,0,1,0, 1,0,0,0, 0,1,0,1,0, 1,0,0,0}, {0,0,1,0,1,0, 0,1,1,1,1,0, 0,1,1,0,0,1, 1,0,1,0, 1,0,1,0, 0,1,0,0};
	currentParams = this->decode(chromosome);

	this->latestScore = this->evaluate(chromosome);
	this->score = latestScore;
}

IMParams BinaryHillClimber::getInitialIMParams()
{
	IMParams params;
	params.TankValue     = 26;
	params.TankRadius    = 10;
	params.MarineValue   = 13;
	params.MarineRadius  = 11;

	params.A = 60;
	params.B = 1;
	params.C = 32;

	params.l = 15;
	params.m = 11;
	params.n = 6;
	return params;
}

void BinaryHillClimber::encode(IMParams params, std::bitset<chromoLength> &chromo)
{
	std::bitset<5> param1 = std::bitset<5>(params.MarineValue);
	std::bitset<4> param2 = std::bitset<4>(params.MarineRadius);
	std::bitset<5> param3 = std::bitset<5>(params.TankValue);
	std::bitset<4> param4 = std::bitset<4>(params.TankRadius);
	
	std::bitset<6> param5 = std::bitset<6>(params.A);
	std::bitset<6> param6 = std::bitset<6>(params.B);
	std::bitset<6> param7 = std::bitset<6>(params.C);

	std::bitset<4> param8 = std::bitset<4>(params.l);
	std::bitset<4> param9 = std::bitset<4>(params.m);
	std::bitset<4> param10= std::bitset<4>(params.n);
	
	int offset = 0;
	for(int i=offset;i<offset+5; i++) {	chromo[i] = param1[4-i+offset]; }
	offset +=5;
	for(int i=offset;i<offset+4; i++) {	chromo[i] = param2[3-i+offset]; }
	offset +=4;
	for(int i=offset;i<offset+5; i++) {	chromo[i] = param3[4-i+offset]; }
	offset +=5;
	for(int i=offset;i<offset+4; i++){  chromo[i] = param4[3-i+offset]; }
	offset +=4;
	for(int i=offset;i<offset+6; i++){	chromo[i] = param5[5-i+offset]; }
	offset +=6;
	for(int i=offset;i<offset+6; i++){	chromo[i] = param6[5-i+offset]; }
	offset +=6;
	for(int i=offset;i<offset+6; i++){	chromo[i] = param7[5-i+offset]; }
	offset +=6;
	for(int i=offset;i<offset+4; i++){	chromo[i] = param8[3-i+offset]; }
	offset +=4;
	for(int i=offset;i<offset+4; i++){	chromo[i] = param9[3-i+offset]; }
	offset +=4;
	for(int i=offset;i<offset+4; i++){	chromo[i] = param10[3-i+offset]; }
}

IMParams BinaryHillClimber::decode(bitset<chromoLength> codes)
{
	int buffer[chromoLength];
	for(int i= 0;i<chromoLength;i++)
		buffer[i] = codes.at(i);

	int param1[]  = {0,0,0,0,0};
	int param2[]  = {0,0,0,0};
	int param3[]  = {0,0,0,0,0};
	int param4[]  = {0,0,0,0};
	int param5[]  = {0,0,0,0,0,0};
	int param6[]  = {0,0,0,0,0,0};
	int param7[]  = {0,0,0,0,0,0};
	int param8[]  = {0,0,0,0};
	int param9[]  = {0,0,0,0};
	int param10[] = {0,0,0,0};

	int offset = 0;
	for(int i=offset;i<offset+5; i++) {	param1[i-offset] = buffer[i]; }
	offset +=5;
	for(int i=offset;i<offset+4; i++) {	param2[i-offset] = buffer[i]; }
	offset +=4;
	for(int i=offset;i<offset+5; i++) {	param3[i-offset] = buffer[i]; }
	offset +=5;
	for(int i=offset;i<offset+4; i++){  param4[i-offset] = buffer[i]; }
	offset +=4;
	for(int i=offset;i<offset+6; i++){	param5[i-offset] = buffer[i]; }
	offset +=6;
	for(int i=offset;i<offset+6; i++){	param6[i-offset] = buffer[i]; }
	offset +=6;
	for(int i=offset;i<offset+6; i++){	param7[i-offset] = buffer[i]; }
	offset +=6;
	for(int i=offset;i<offset+4; i++){	param8[i-offset] = buffer[i]; }
	offset +=4;
	for(int i=offset;i<offset+4; i++){	param9[i-offset] = buffer[i]; }
	offset +=4;
	for(int i=offset;i<offset+4; i++){	param10[i-offset] = buffer[i]; }

	currentParams.TankValue     = decode(param1, 5);
	currentParams.TankRadius    = decode(param2, 4);
	currentParams.MarineValue   = decode(param3, 5);
	currentParams.MarineRadius  = decode(param4, 4);

	currentParams.A = decode(param5, 6);
	currentParams.B = decode(param6, 6);
	currentParams.C = decode(param7, 6);

	currentParams.l = decode(param8, 4);
	currentParams.m = decode(param9, 4);
	currentParams.n = decode(param10, 4);
	

	return currentParams;
}

int BinaryHillClimber::decode(int param[], int length)
{
	int result = 0;
	for(int i=0;i<length;i++)
	{
		result += (int)(param[i])*pow((float)2, length-i-1);
	}
	return result;
}

void BinaryHillClimber::randomFlip(std::bitset<chromoLength> &paramsBS, int type)
{
	
	return; //no change

	int pos = 0;
	if(type == 0) //flip in all position
	{
		pos = rand()%chromoLength;
	}
	else if(type == 1)  //flip im only
	{
		pos = rand()%imLength;
	}
	else if(type == 2)  //flip pf only
	{
		pos = imLength + rand()%pfLength;
	}
	paramsBS = paramsBS.flip(pos);
}

double BinaryHillClimber::evaluate(std::bitset<chromoLength> paramsBS)
{
	double result = 0.0;
	int chromo[] = {0,0,0,0,0, 0,0,0,0, 0,0,0,0,0, 0,0,0,0,  0,0,0,0,0,0, 0,0,0,0,0,0, 0,0,0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0};
	int length = paramsBS.size();

    for(int i = 0; i < paramsBS.size(); i++){
	  chromo[i] = paramsBS.at(i);
    }
	
	result = this->sharedMemoryEvaluator->eval(chromo, length);
	return result;
}

void BinaryHillClimber::log2file(int evaluateCount)
{
	string logfile = "c:\\lsm\\BinaryHillClimber";
	if(this->type == 1)
		logfile += "_im_";
	else if(this->type == 2)
		logfile += "_pf_";
	else 
		logfile += "_all_";

	logfile += ".log";

	ofstream o_file;
	o_file.open(logfile.c_str(), ios::app);
	IMParams p = this->decode(this->chromosome);

	//output to screen
	std::cout<< evaluateCount << " " << this->score << " " << this->latestScore << " "<< p.TankValue<<" "<<p.TankRadius   <<" "<<p.MarineValue <<" "<<p.MarineRadius  <<" "<<
		p.A<<" "<<p.B<<" "<<p.C<<" "<<p.l<<" "<<p.m<<" "<<p.n<< endl;

	//output to file
	o_file<< evaluateCount << " " << this->score << " " << this->latestScore << " "<< p.TankValue<<" "<<p.TankRadius   <<" "<<p.MarineValue <<" "<<p.MarineRadius  <<" "<<
		p.A<<" "<<p.B<<" "<<p.C<<" "<<p.l<<" "<<p.m<<" "<<p.n<< endl;
	o_file.close();
}

void BinaryHillClimber::run()
{
	for(int i=0; i< maxCount; i++)
	{
		bitset<chromoLength> chromo_new = this->chromosome;
		this->randomFlip(chromo_new, this->type); //random flip
		this->latestScore = this->evaluate(chromo_new); //evaluate
		if(this->latestScore > this->score)        //climb if the newer one is better
		{
			this->score = this->latestScore;
			this->chromosome = chromo_new;
		}

		log2file(i+1);
	}
}

BinaryHillClimber::~BinaryHillClimber()
{
	delete sharedMemoryEvaluator;
}