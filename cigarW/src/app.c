#pragma once

#include<windows.h>
#include <math.h>
#include <stdio.h>
#include "type.h"

/**********original evaluation function*****************
double Eval(IPTR pj)
{
  int i;
  double sum;

  sum = 0.0;
  for(i = 0; i < pj->chromLen; i++){
    sum += pj->chrom[i];

  }
  return sum;
}
******************************************************/

int uv, ur,tv,tr,A,B,m,n,m1,m2,m3,m4,m5,m6;

double Eval1(IPTR pj)
{
  int i;
  int chromoLen = pj->chromLen;
  int signalLen = 4;
  double result = 0.0;

  int buffer[] = {0,0,0,0,  
	   0,0,0,0,0, 0,0,0,0, 0,0,0,0,0, 0,0,0,0,  
	   0,0,0,0,0,0, 0,0,0,0,0,0, 0,0,0,0, 0,0,0,0, 
	   0,0,0,0, 0,0,0,0,0, 0,0,0,0, 0,0,0, 0,0,0, 0,0,0
  };
  int values[] = {0,0,0,0};

  for(i = 0; i < pj->chromLen; i++){
    result += pj->chrom[i];
	buffer[i+signalLen] = pj->chrom[i];
  }

  pBuf = (int *) MapViewOfFile(hMapFile,   // handle to map object
                        FILE_MAP_ALL_ACCESS, // read/write permission
                        (DWORD)0,
                        (DWORD)0,
                        BUF_SIZE);

  //set the individual as input to evaluate
  CopyMemory((PVOID)pBuf, buffer, (pj->chromLen+signalLen)*sizeof(int));
  UnmapViewOfFile(pBuf);
  
  //get the fitness from  the bwapi side
  while(1)
  {
	  pBuf = (int*) MapViewOfFile(hMapFile, // handle to map object
               FILE_MAP_ALL_ACCESS,  // read/write permission
               0,
               0,
               BUF_SIZE);
	
	  CopyMemory(values, (PVOID)pBuf, sizeof(int)*signalLen);

	  if(values[1]>0 && values[2] >0 &&values[3]>0)
	  {
		  result = values[0];
		  UnmapViewOfFile(pBuf);
		  break;
	  }
	  UnmapViewOfFile(pBuf);
	  Sleep(1);
  }

  return result;
}

double Eval(IPTR pj){
	double ss, avg;
	int i;
	ss = 0;
	for (i=0;i<10;i++){
		ss += Eval1(pj);
	}
	
	avg = ss/10;
	return avg;
}


void AppInitChrom(IPTR pj)
{
  return;
}

void AppInit(char *appInfile)
{
	initSharedMemory();
}

void PhenoPrint2(FILE *fp, IPTR pj, Population *p)
{
	int i;
    for(i = 0; i < pj->chromLen; i++){
		fprintf(fp, "%1i", pj[p->maxi].chrom[i]);
	}
	fprintf(fp, "\n");
}

void parseParameterInput(int buffer[], int length)
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
	
	

	int offset = 0;
	int i;
	for(i=offset;i<offset+5; i++) {	param1[i-offset] = buffer[i]; }
	offset +=5;
	for(i=offset;i<offset+4; i++) {	param2[i-offset] = buffer[i]; }
	offset +=4;
	for(i=offset;i<offset+5; i++) {	param3[i-offset] = buffer[i]; }
	offset +=5;
	for(i=offset;i<offset+4; i++){  param4[i-offset] = buffer[i]; }
	offset +=4;

	for(i=offset;i<offset+6; i++){	param5[i-offset] = buffer[i]; }
	offset +=6;
	for(i=offset;i<offset+6; i++){	param6[i-offset] = buffer[i]; }
	offset +=6;
	for(i=offset;i<offset+4; i++){	param7[i-offset] = buffer[i]; }
	offset +=4;
	for(i=offset;i<offset+4; i++){	param8[i-offset] = buffer[i]; }
	offset +=4;

	for(i=offset;i<offset+4; i++){	param9[i-offset] = buffer[i]; }
	offset +=4;
	for(i=offset;i<offset+5; i++){	param10[i-offset] = buffer[i]; }
	offset +=5;
	for(i=offset;i<offset+4; i++){	param11[i-offset] = buffer[i]; }
	offset +=4;
	for(i=offset;i<offset+3; i++){	param12[i-offset] = buffer[i]; }
	offset +=3;
	for(i=offset;i<offset+3; i++){	param13[i-offset] = buffer[i]; }
	offset +=3;
	for(i=offset;i<offset+3; i++){	param14[i-offset] = buffer[i]; }

	
	uv = decode(param1, 5)+1;
	ur = decode(param2, 4)+1;
	tv = decode(param3, 5)+1;
	tr = decode(param4, 4)+1;

	A = decode(param5, 6)+1;
	B = decode(param6, 6)+1;

	m = decode(param7, 4)+1;
	n = decode(param8, 4)+1;

	m1 = decode(param9,  4);
	m2 = decode(param10, 5);
	m3 = decode(param11, 4);
	m4 = decode(param12, 3);
	m5 = decode(param13, 3);
	m6 = decode(param14, 3);
}

int decode(int param[], int length)
{
	int result = 0;
	int i;
	for(i=0;i<length;i++)
	{
		result += (int)(param[i])*pow((float)2, length-i-1);
	}
	return result;
}

void PhenoPrint(FILE *fp, IPTR pj, Population *p)
{
	parseParameterInput(pj[p->maxi].chrom, pj->chromLen);
	fprintf(fp, "%2i,%2i,%2i,%2i,%2i,%2i,%2i,%2i,%2i,%2i,%2i,%2i,%2i,%2i", uv, ur, tv, tr, A, B, m, n, m1, m2,m3,m4,m5,m6);
	fprintf(fp, "\n");

}
void InitPhenoPrint(IPTR pj, char *fname, Population *px)
{

}

void initSharedMemory()
{
	TCHAR szName[]=TEXT("MachineLearningParameters");
	char buffer[] = "Initialized";

	hMapFile = CreateFileMapping(
                 INVALID_HANDLE_VALUE,    // use paging file
                 NULL,                    // default security
                 PAGE_READWRITE,          // read/write access
                 (DWORD)0,                // maximum object size (high-order DWORD)
                 (DWORD)BUF_SIZE,         // maximum object size (low-order DWORD)
                 (LPCTSTR) &szName[0]);   // name of mapping object


	if (hMapFile == NULL)
    {
       _tprintf(TEXT("Could not create file mapping object (%d).\n"), GetLastError());
       return;
    }

}

void destroySharedMemory()
{
	CloseHandle(hMapFile);
}