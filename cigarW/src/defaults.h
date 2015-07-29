#ifndef __DEFAULTS_H__
#define __DEFAULTS_H__

#define SEED                  0.3//0.53284319

#define LAMBDA                2
#define POPSIZE               30
#define MAXGEN                40
//#define CHROMLENGTH           50
#define CHROMLENGTH           60    //8 params, each with 4 bits

#define PCROSS                0.88  //0.66
#define PMUT                  0.01  //0.001
#define NXPOINTS              4     //2
#define SCALEFACTOR           1.2

#define CROSSOVER             NPointCrossover
#define CURRENTGA             CHC //Generation0
#define FINDMATE              Roulette

#define OUTPUTFILE            "ofile"
#define CASEFILE              "cb"
#define TRUE                  (1 == 1)
#define FALSE                 (0 == 1)

#endif
