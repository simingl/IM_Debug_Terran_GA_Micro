#pragma once

#include "BinaryHillclimber.h"

int main(int argc, const char* argv[])
{
	if(argc == 3)
	{
		int type = atoi( argv[1] );
		int seed = atoi( argv[2] );
		
		BinaryHillClimber* bhc = new BinaryHillClimber(type, seed);
		bhc->run();
	}
	else 
	{
		std::cout<<"Please launch with args( hill_climber.exe <type> <seed>): "<<std::endl;
		std::cout<<"type - 0: flip all, 1: flip IM, 2: flip PF"<<std::endl;	
	}
}