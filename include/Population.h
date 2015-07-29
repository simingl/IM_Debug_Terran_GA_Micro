#pragma once

#include "GA.h"
#include "Individual.h"

class Individual
{
public:
//	IMParamsGA *params;

	std::bitset<32> chrom;
	int chromLen;
	double fitness;
	double scaledFitness;

	double calculateFitness();

//	IMParams *parent1;
//	IMParams *parent2;
};

class Population
{
public:
	Individual *oldpop;
	Individual *newpop;

	double seed;
	int maximize;
	double maxConst;

	int generation;
	int maxgen;
	int popsize;
	int chromLength;
	double max, min, avg;
	int maxi, mini, bigMaxi;
	double bigMax;
	int bigGen, bigInd;

	double sumFitness, saaledSumFitness;
	double smax, smin;
	double scaleConstA, scaleConstB;

	float pCross, pMut;
	int nXPoints;

	float scaleFactor;	
};