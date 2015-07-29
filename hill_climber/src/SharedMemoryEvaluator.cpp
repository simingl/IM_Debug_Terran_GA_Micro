#pragma once

#include "SharedMemoryEvaluator.h"

SharedMemoryEvaluator::SharedMemoryEvaluator()
{
}

SharedMemoryEvaluator::~SharedMemoryEvaluator()
{
	this->destroy();
}

void SharedMemoryEvaluator::initialize()
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
		printf("Could not create file mapping object (%d).\n",GetLastError());
       //_tprintf(TEXT("Could not create file mapping object (%d).\n"), GetLastError());
       return;
    }

	pBuf = (int *) MapViewOfFile(hMapFile,   // handle to map object
                        FILE_MAP_ALL_ACCESS, // read/write permission
                        (DWORD)0,
                        (DWORD)0,
                        BUF_SIZE);

	if (pBuf == NULL)
    {
		printf("Could not map view of file (%d).\n", GetLastError());
        //_tprintf(TEXT("Could not map view of file (%d).\n"),GetLastError());
        CloseHandle(hMapFile);
        return;
    }

    CopyMemory((PVOID)pBuf,buffer, BUF_SIZE);
}

double SharedMemoryEvaluator::eval(int chromo[], int length)
{
	this->initialize();

	  int i;
	  int chromoLen = length;
	  int signalLen = 4;
	  double result = 0.0;

	  int buffer[] = {0,0,0,0,  0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0,  0,0,0,0,0,0, 0,0,0,0,0,0, 0,0,0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0}; 
	  int values[] = {0,0,0,0};

	  for(i = 0; i < chromoLen; i++){
		  int c = chromo[i];
		  buffer[i+signalLen] = chromo[i];
	  }

	  //set the individual as input to evaluate
	  CopyMemory((PVOID)pBuf, buffer, (chromoLen+signalLen)*sizeof(int));

	  
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

void SharedMemoryEvaluator::destroy()
{
	UnmapViewOfFile(pBuf);
	CloseHandle(hMapFile);
}

