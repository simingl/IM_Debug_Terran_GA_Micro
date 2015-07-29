#pragma once

#include "BSO.h"

int main(int argc, const char* argv[])
{
	if(argc == 2)
	{
		int seed = atoi( argv[1] );
		
		BSO* bso = new BSO(0, seed);
		bso->run();
	}
	else 
	{
		std::cout<<"Please launch with args( BSO.exe <seed>): "<<std::endl;
	}
}