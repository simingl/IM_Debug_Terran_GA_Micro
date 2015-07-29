#pragma once

#include "BinaryHillClimberAvg.h"

using namespace std;

BinaryHillClimberAvg::BinaryHillClimberAvg()
{
	init2average();
}

void BinaryHillClimberAvg::init2average()
{
	bool lastExit = this->readfile();

	if (!lastExit){
		evaluateCount=0;
		score = 0;
		seed = 0;
        
		this->generateRandomParameter(1);

		this->updateDate2file();
	}
	else
	{
		if(this->evaluateCount < 10) //in one seed
		{ 
			this->evaluateCount++;
			this->seed = seed + this->tempScore;   //seed used to accumulate
		}
		else
		{
			this->score = this->seed / 10;         //this is the everage of last time;
			this->seed = 0;
 			this->evaluateCount=0;
			this->modify();


		}
		this->updateDate2file();
	}
}

void BinaryHillClimberAvg::generateRandomParameter(int seed)
{
		srand(seed); //set rand seed

		int valueRange = 16;
		int radiusRange = 16;
		//Mine units
		currentParams.m_GoliathValue = rand()%valueRange; //[0,16)
		currentParams.m_TTankValue   = rand()%valueRange;
		currentParams.m_VultureValue = rand()%valueRange;
		currentParams.m_STankValue   = rand()%valueRange;
		currentParams.m_GoliathRadius= rand()%radiusRange; //[0,16)
		currentParams.m_TTankRadius  = rand()%radiusRange;
		currentParams.m_VultureRadius= rand()%radiusRange;
		currentParams.m_STankRadius  = rand()%radiusRange;

		this->adjustedParams = currentParams; 
}
IMParams BinaryHillClimberAvg::getCurrentParams()
{
	return currentParams;
}

IMParams* BinaryHillClimberAvg::getAdjustedParams()
{
	return &adjustedParams;
}

void BinaryHillClimberAvg::modify()
{
	bitset<32> bs = this->encode(this->getCurrentParams());
	bs = this->randomFlip(bs);
	this->adjustedParams = this->decode(bs);
}

bitset<32> BinaryHillClimberAvg::encode(IMParams params)
{
	bitset<4> mgv(params.m_GoliathValue);
	bitset<4> mtv(params.m_TTankValue);
	bitset<4> mvv(params.m_VultureValue);
	bitset<4> msv(params.m_STankValue);

	bitset<4> mgr(params.m_GoliathRadius);
	bitset<4> mtr(params.m_TTankRadius);
	bitset<4> mvr(params.m_VultureRadius);
	bitset<4> msr(params.m_STankRadius);

    bitset<32> result;
	int offset = 0;
	//mine value
	for(int i=0; i<mgv.size(); i++){result[offset++] = mgv[i];}
	for(int i=0; i<mtv.size(); i++){result[offset++] = mtv[i];}	
	for(int i=0; i<mvv.size(); i++){result[offset++] = mvv[i];}
	for(int i=0; i<msv.size(); i++){result[offset++] = msv[i];}
	//mine radius
	for(int i=0; i<mgr.size(); i++){result[offset++] = mgr[i];}
	for(int i=0; i<mtr.size(); i++){result[offset++] = mtr[i];}
	for(int i=0; i<mvr.size(); i++){result[offset++] = mvr[i];}	
	for(int i=0; i<msr.size(); i++){result[offset++] = msr[i];}

	return result;
}

IMParams BinaryHillClimberAvg::decode(bitset<32> codes)
{
	bitset<4> mgv, mtv, mvv, msv;
	bitset<4> mgr, mtr, mvr, msr;
	//bitset<4> egv, etv, evv, esv;
	//bitset<6> egr, etr, evr, esr;

	int offset = 0;
	//mine value
	for(int i=0; i<mgv.size(); i++){mgv[i] = codes[offset++];}
	for(int i=0; i<mtv.size(); i++){mtv[i] = codes[offset++];}	
	for(int i=0; i<mvv.size(); i++){mvv[i] = codes[offset++];}
	for(int i=0; i<msv.size(); i++){msv[i] = codes[offset++];}
	//mine radius
	for(int i=0; i<mgr.size(); i++){mgr[i] = codes[offset++];}
	for(int i=0; i<mtr.size(); i++){mtr[i] = codes[offset++];}
	for(int i=0; i<mvr.size(); i++){mvr[i] = codes[offset++];}	
	for(int i=0; i<msr.size(); i++){msr[i] = codes[offset++];}

	IMParams params;
	params.m_GoliathValue  = (int)mgv.to_ulong();
	params.m_TTankValue    = (int)mtv.to_ulong();
	params.m_VultureValue  = (int)mvv.to_ulong();
	params.m_STankValue    = (int)msv.to_ulong();

	params.m_GoliathRadius = (int)mgr.to_ulong();
	params.m_TTankRadius   = (int)mtr.to_ulong();
	params.m_VultureRadius = (int)mvr.to_ulong();
	params.m_STankRadius   = (int)msr.to_ulong();

	return params;
}
bitset<32> BinaryHillClimberAvg::randomFlip(bitset<32> paramsBS)
{
	int pos = rand()%32;
	paramsBS = paramsBS.flip(pos);
	return paramsBS;
}

void BinaryHillClimberAvg::setScore(int score)
{
	//this->score = score;
}

void BinaryHillClimberAvg::setTempScore(int s)
{
	this->tempScore = s;
}

void BinaryHillClimberAvg::useAdjustedParam()
{
	this->currentParams = this->adjustedParams;
}

void BinaryHillClimberAvg::record2file()
{
	this->updateDate2file();
	this->log2file();
}

bool BinaryHillClimberAvg::readfile()
{
	//file format: count, score, tempScore, Goliath Value, Tank Value, Vulture Value, Siege Tank Value, GRadius, TRadius, VRadius, STRadius
	const char filename[] = "c:\\lsm\\BinaryHillClimberAvg.data";
	ifstream i_file;
    i_file.open(filename);
	if(i_file)
	{
		i_file>>this->seed;
		i_file>>this->evaluateCount;
		i_file>>this->score;
		i_file>>this->tempScore;

		i_file>>this->currentParams.m_GoliathValue;
		i_file>>this->currentParams.m_TTankValue;
		i_file>>this->currentParams.m_VultureValue;
		i_file>>this->currentParams.m_STankValue;

		i_file>>this->currentParams.m_GoliathRadius;
		i_file>>this->currentParams.m_TTankRadius;
		i_file>>this->currentParams.m_VultureRadius;
		i_file>>this->currentParams.m_STankRadius;

		i_file.close();

		return true;
	}
	else
	{
		return false;
	}
}

void BinaryHillClimberAvg::updateDate2file()
{
	const char datafile[] = "c:\\lsm\\BinaryHillClimberAvg.data";
	ofstream o_file;
	o_file.open(datafile);
	o_file << this->seed <<" " << evaluateCount << " " << this->score << " " << this->tempScore << " " << currentParams.m_GoliathValue<<" "<<currentParams.m_TTankValue   <<" "<<currentParams.m_VultureValue <<" "<<currentParams.m_STankValue  <<" "<<currentParams.m_GoliathRadius<<" "<<
		currentParams.m_TTankRadius <<" "<<currentParams.m_VultureRadius<<" "<<currentParams.m_STankRadius<<endl;
//		<<" "<<currentParams.e_GoliathValue<<" "<<currentParams.e_TTankValue<<" "<<
//		currentParams.e_VultureValue<<" "<<currentParams.e_STankValue   <<" "<<currentParams.e_GoliathRadius<<" "<<currentParams.e_TTankRadius <<" "<<currentParams.e_VultureRadius<<" "<<	
//		currentParams.e_STankRadius << endl; 
	o_file.close();
}

void BinaryHillClimberAvg::log2file()
{
	const char logfile[] = "c:\\lsm\\BinaryHillClimberAvg.log";
	ofstream o_file;
	o_file.open(logfile, ios::app);
	//o_file.open(filename);
	o_file<< this->seed <<" " << evaluateCount << " " << this->score << " " << this->tempScore << " " << currentParams.m_GoliathValue<<" "<<currentParams.m_TTankValue   <<" "<<currentParams.m_VultureValue <<" "<<currentParams.m_STankValue  <<" "<<currentParams.m_GoliathRadius<<" "<<
		currentParams.m_TTankRadius <<" "<<currentParams.m_VultureRadius<<" "<<currentParams.m_STankRadius<< endl;
		//<<" "<<currentParams.e_GoliathValue<<" "<<currentParams.e_TTankValue<<" "<<
		//currentParams.e_VultureValue<<" "<<currentParams.e_STankValue   <<" "<<currentParams.e_GoliathRadius<<" "<<currentParams.e_TTankRadius <<" "<<currentParams.e_VultureRadius<<" "<<	
		//currentParams.e_STankRadius << endl; 
	o_file.close();
}