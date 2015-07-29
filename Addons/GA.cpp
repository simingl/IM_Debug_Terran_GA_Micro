#pragma once

#include "GA.h"

using namespace std;


const TCHAR GA::szName[]=TEXT("MachineLearningParameters");
const char GA::datafile[] = "c:\\lsm\\GA.data";
const char GA::logfile[] = "c:\\lsm\\GA.log";
const char GA::conffile[] = "c:\\immicro.conf";

GA * GA::instance = 0;  

GA* GA::getInstance()  
{  
    if (instance == 0) {  
        instance = new GA();  
    }  
 
    return instance;  
} 

GA::GA()
{
	chromoLen = 60;
	signalLen = 4;
	this->initialized = false;
	initSharedMemory();
}

void GA::setScore(int score)
{
	this->score = score; 
}

int GA::initSharedMemory()
{	
   hMapFile = OpenFileMapping(
                   FILE_MAP_ALL_ACCESS,   // read/write access
                   FALSE,                 // do not inherit the name
                   szName);               // name of mapping object

   if (hMapFile == NULL)
   {
      _tprintf(TEXT("Could not open file mapping object (%d).\n"),
             GetLastError());
      return 1;
   }

   return 0;
}

void GA::testInit(int buffer[])
{
	char* chrom = "010001100111010111000101011110000010111111010101";
	for(int i=0;i<signalLen;i++){
		buffer[i] = 0;
	}
	for(int i=0;i<chromoLen;i++){
		buffer[i+signalLen] = chrom[i];
	}
}

void GA::readParams(){
	ifstream cf;
	cf.open(this->conffile);
	string line;
	string melee_line;
	string range_line;
	string key_melee = "melee=";
	string key_range = "range=";
	if(cf.is_open()){
		while(!cf.eof()){
			cf >> line;
			int pos_melee = line.find(key_melee);
			if(pos_melee>=0) {
				melee_line = line.substr(pos_melee+key_melee.length());
				continue;
			}
			int pos_range = line.find(key_range);
			if(pos_range>=0) {
				range_line = line.substr(pos_range+key_range.length());
				continue;
			}
		}
	}
	cf.close();

	//read melee parameters
	std::istringstream ssm(melee_line);
	std::string token;
	int params[14];
	int i = 0;
	while(std::getline(ssm, token, ',')) {
		params[i++] = atoi(token.c_str());
	}

	currentParams.UnitValue      = params[0];
	currentParams.UnitRadius     = params[1];
	currentParams.TerrainValue   = params[2];
	currentParams.TerrainRadius  = params[3];

	currentParams.A = params[4];
	currentParams.B = params[5];

	currentParams.m = params[6];
	currentParams.n = params[7];

	currentParams.m_freeze      = params[8];
	currentParams.m_kitingRange = params[9];
	currentParams.m_targetNearby= params[10];
	currentParams.m_kitingDist  = params[11];
	currentParams.m_hpfocus     = params[12];
	currentParams.m_hpkiting    = params[13];

	//read range parameters
	std::istringstream ssr(range_line);
	i = 0;
	while(std::getline(ssr, token, ',')) {
		params[i++] = atoi(token.c_str());
	}

	currentParams_range.UnitValue      = params[0];
	currentParams_range.UnitRadius     = params[1];
	currentParams_range.TerrainValue   = params[2];
	currentParams_range.TerrainRadius  = params[3];

	currentParams_range.A = params[4];
	currentParams_range.B = params[5];

	currentParams_range.m = params[6];
	currentParams_range.n = params[7];

	currentParams_range.m_freeze      = params[8];
	currentParams_range.m_kitingRange = params[9];
	currentParams_range.m_targetNearby= params[10];
	currentParams_range.m_kitingDist  = params[11];
	currentParams_range.m_hpfocus     = params[12];
	currentParams_range.m_hpkiting    = params[13];
}

IMParams GA::getParams()
{
	
	//////////////////////////////////////////////////////////////
	
	//Vulture vs Vulture:  23,  22,  21,   7,  48,   1,   8,   2,   5,  21,   3,   4,   3,   7
	//                     24, 14,  8,  3; 20, 51, 16,  4;  2, 14,  5,  1,  7,  2
	//                     13, 13, 11,  2; 27, 59, 16,  2; 11, 21,  4,  5,  3,  3
	//5 Vul vs 6 Vul(GOOD): 25, 12, 21, 10; 34, 17, 16,  4; 13,  8,  2,  4,  0,  7
	//V v V, against UAlberta - 245 - 5 average,   23, 10, 22,  7; 40, 11, 14,  4; 10, 14,  1,  6,  3,  0
	//                                             31, 10, 22,  7; 36, 11, 14,  3; 10, 14,  5,  6,  3,  0
	//Vulture vs Zealot:  30, 15,  3, 11; 57,48,1,14; 0,30,6,1,7,1   - 4000 destroy score
	//3720 destroy score:  3,  9, 15,  8; 43, 55,  6,  2;  1,  5,  6,  7,  7,  0
	//Zealot vs Zealot: 26, 16, 23, 12; 46,  5,  4,  2;  7,  8,  3,  2,  0,  4

	this->initialized = true;
	readParams();

	//////////////////////////////////////////////////////////////
	if (!this->initialized)
		this->readParameterFromSharedMemory();

	return this->currentParams;
}

IMParams GA::getParamsRange()
{
	return this->currentParams_range;
}


void GA::readParameterFromSharedMemory()
{
	this->initialized = true;

   int buffer[] = {0,0,0,0,  
	   0,0,0,0,0, 0,0,0,0, 0,0,0,0,0, 0,0,0,0,  
	   0,0,0,0,0,0, 0,0,0,0,0,0, 0,0,0,0, 0,0,0,0, 
	   0,0,0,0, 0,0,0,0,0, 0,0,0,0, 0,0,0, 0,0,0, 0,0,0
   };

   while(1)
   {
	   pBuf = (int*) MapViewOfFile(hMapFile, // handle to map object
               FILE_MAP_ALL_ACCESS,  // read/write permission
               0,
               0,
               BUF_SIZE);

       if (pBuf == NULL)
       {
            _tprintf(TEXT("Could not map view of file (%d).\n"), GetLastError());
            Sleep(1000);   //server not start yet.
	   }
	   else
	   {
		   //get parameters from shared memory
		   CopyMemory(buffer, (PVOID)pBuf , (chromoLen+signalLen)*sizeof(int));
		   
		   bool serverRefreshed = true;
		   for(int j= 0; j < signalLen;j++)
		   {
			   if(buffer[j] > 0) serverRefreshed = false;
		   }
		   
		   UnmapViewOfFile(pBuf);

		   if(serverRefreshed) 
		   {
			   break;  //break the loop, stop wait, start to evaluate
		   }
		   else
		   {
			   Sleep(100);
		   }
	   }
   }

   parseParameterInput(buffer, chromoLen+signalLen);

   //use the best solution to test
   //testInit(buffer);

}

void GA::parseParameterInput(int buffer[], int length)
{
	int param1[]  = {0,0,0,0,0};
	int param2[]  = {0,0,0,0};
	int param3[]  = {0,0,0,0,0};
	int param4[]  = {0,0,0,0};
	
	int param5[]  = {0,0,0,0,0,0};
	int param6[]  = {0,0,0,0,0,0};
	int param7[]  = {0,0,0,0};
	int param8[]  = {0,0,0,0};

	int param9[]  = {0,0,0,0};
	int param10[] = {0,0,0,0,0};
	int param11[] = {0,0,0,0};
	int param12[] = {0,0,0};
	int param13[] = {0,0,0};
	int param14[] = {0,0,0};

	int offset = 4;
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
	for(int i=offset;i<offset+4; i++){	param7[i-offset] = buffer[i]; }
	offset +=4;
	for(int i=offset;i<offset+4; i++){	param8[i-offset] = buffer[i]; }
	offset +=4;

	for(int i=offset;i<offset+4; i++){	param9[i-offset] = buffer[i]; }
	offset +=4;
	for(int i=offset;i<offset+5; i++){	param10[i-offset] = buffer[i]; }
	offset +=5;
	for(int i=offset;i<offset+4; i++){	param11[i-offset] = buffer[i]; }
	offset +=4;
	for(int i=offset;i<offset+3; i++){	param12[i-offset] = buffer[i]; }
	offset +=3;
	for(int i=offset;i<offset+3; i++){	param13[i-offset] = buffer[i]; }
	offset +=3;
	for(int i=offset;i<offset+3; i++){	param14[i-offset] = buffer[i]; }

	currentParams.UnitValue     = decode(param1, 5)+1;
	currentParams.UnitRadius    = decode(param2, 4)+1;
	currentParams.TerrainValue  = decode(param3, 5)+1;
	currentParams.TerrainRadius = decode(param4, 4)+1;

	currentParams.A = decode(param5, 6)+1;
	currentParams.B = decode(param6, 6)+1;

	currentParams.m = decode(param7, 4)+1;
	currentParams.n = decode(param8, 4)+1;

	currentParams.m_freeze      = decode(param9,  4);
	currentParams.m_kitingRange = decode(param10, 5);
	currentParams.m_targetNearby= decode(param11, 4);
	currentParams.m_kitingDist  = decode(param12, 3);
	currentParams.m_hpfocus     = decode(param13, 3);
	currentParams.m_hpkiting    = decode(param14, 3);
}

int GA::decode(int param[], int length)
{
	int result = 0;
	for(int i=0;i<length;i++)
	{
		result += (int)(param[i])*pow((float)2, length-i-1);
	}
	return result;
}

int GA::writeFitness2SharedMemory(int score)
{
	//return 0; //pause to use GA

   pBuf = (int*) MapViewOfFile(hMapFile, // handle to map object
               FILE_MAP_ALL_ACCESS,  // read/write permission
               0,
               0,
               BUF_SIZE);

   if (pBuf == NULL)
   {
      _tprintf(TEXT("Could not map view of file (%d).\n"),
             GetLastError());

      CloseHandle(hMapFile);

      return 1;
   }

   int buffer[] = {score,1,1,1}; // 0000-cmd 0000*8 = 8 parameters

   CopyMemory((PVOID)pBuf, buffer, signalLen*sizeof(int));

   UnmapViewOfFile(pBuf);
   CloseHandle(hMapFile);

   return 0;
}

void GA::log2file(int score)
{
	std::ofstream o_file;
	o_file.open(logfile, std::ios::app);
	o_file << score << std::endl;
	o_file.close();
}