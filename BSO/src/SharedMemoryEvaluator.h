#pragma once

#include<windows.h>
#include<cstdio>

class SharedMemoryEvaluator
{
	public:
		SharedMemoryEvaluator();
		double eval(int chromo[], int length);
		~SharedMemoryEvaluator();
		

	private:
		void initialize();
		void destroy();

		#define BUF_SIZE 256

		HANDLE hMapFile;
		int* pBuf;

};