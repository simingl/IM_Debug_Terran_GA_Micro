#pragma once

#include <stdio.h>
#include <math.h>
#include <stdlib.h>

#include "type.h"
#include "random.h"
#include "utils.h"
#include "case.h"
#include "finders.h"

IPTR caseBase;
int *sIndex, *hamDist;

double Eval(IPTR pj);

void LoadCases(IPTR pop, int gen, float frac, Population *p, Functions *f)
{
  int nseeds, newSeeds, i, index;
  int *rank;
  IPTR seedCases;

  if(p->nCases <= 0) return;
  if(p->injectFraction   <= 0.0) return;
  nseeds = (int) ((float) p->popsize * (float) p->injectFraction);
  if (p->nCases < nseeds) {
    nseeds = p->nCases;
  }

  rank = (int *) malloc (sizeof(int) * p->popsize);
  if (rank == NULL) {
    perror("error in malloc (rank)\n"); 
  }

  seedCases = AllocateIndividuals(nseeds, p->chromLength);
  /* (IPTR) malloc (sizeof(INDIVIDUAL) * nseeds);*/
  if (seedCases == NULL) {
    perror("error in malloc (seedCases)\n"); 
  }

  index = f->GetIndexIndividual(pop, p->popsize);
  /*   printf("Before GetCases\n");*/

  newSeeds = GetCases( &pop[index], seedCases, nseeds, p, f);
  /* newSeeds may be less than nseeds */
  /*  printf("after GetCases\n");*/
#ifdef DEBUG
  PrintCases(debugFName, seedCases, nseeds, pop, best);
#endif

  for(i = 0; i < p->popsize; i++) {
    rank[i] = i;
  }
  FindNWorst(pop, rank, p->popsize, newSeeds);
  /*  printf("after FindNworst %d\n", nseeds);  */
  for(i = 0; i < newSeeds; i++){
    IndividualCopy(&seedCases[i], &pop[rank[i]]); 
    /*    pop[rank[i]].fitness = eval_org(&pop[rank[i]]); */
    /* Done in GetCases */
  }
  free (rank);
  free (seedCases);
}

int GetBest(IPTR pop, int size)
{
  int i;
  int max = 0;
  for(i = 1; i < size; i++){
    if(pop[i].fitness > pop[max].fitness){
      max = i;
    }
  }
  return max;
}

int GetWorst(IPTR pop, int size)
{
  int i;
  int min = 0;
  for(i = 1; i < size; i++){
    if(pop[i].fitness < pop[min].fitness){
      min = i;
    }
  }
  return min;
}

void SaveCase(FILE *fp, IPTR pj, int gen, Population *p)
{
  int i;

  if(p->saveCases) {
    fprintf(fp, "%5d ", gen);
    for(i = 0; i < p->chromLength ; i++) {
      fprintf(fp, "%i ", pj->chrom[i]);
    }
    fprintf(fp, " %f %f\n", pj->fitness, pj->scaledFitness);
    p->nCurrentCases++;
  }
  return;
}

int GetCases(IPTR pj, IPTR iCases, int howmany, Population *p, Functions *f)
{
  int i, j, ncopied = 0;
  IPTR tmp;
  FILE *fp;
  IPTR cbj;
  int *sIndex;
  double *dist;
  
  if(p->nCases <= 0) 
    return 0;
  
  sIndex = (int *) malloc (sizeof(int) * p->nCases);
  if (sIndex == NULL) {
    perror("error in malloc (sIndex)\n"); 
  }
  dist = (double *) malloc (sizeof(double) * p->nCases);
  if (dist == NULL) {
    perror("error in malloc (dist)\n"); 
  }
  for(i = 0; i < p->nCases; i++) {
    dist[i] = 
      f->DistanceMetric(pj->chrom, (&(caseBase[i]))->chrom, p->chromLength);
  }
  for(i = 0; i < p->nCases; i++) {
    sIndex[i] = i;
  }
  f->ApplyMetric(dist, sIndex, p->nCases, howmany);


  ncopied = 0;
  for(i = 0; 
      (i < p->nCases) && (ncopied < howmany) && (ncopied < p->nCases); 
      i++) { 
    if(dist[sIndex[i]] != 0.0) { 
      IndividualCopy(&caseBase[sIndex[i]], &iCases[ncopied]);
      iCases[ncopied].fitness = Eval(&(iCases[ncopied]));
      iCases[ncopied].dx = dist[sIndex[i]];
      ncopied++;
    }
  }
  /*************************************************************  
  ncopied = 0;
  for(i = 0; i < howmany; i++) {
    IndividualCopy(&caseBase[sIndex[i]], &iCases[i]);
    iCases[i].fitness = Eval(&(iCases[i]));
    iCases[i].dx = dist[sIndex[i]];
    ncopied++;
  }
  *************************************************************/
  
  free (sIndex);
  free (dist);
  return ncopied;
}

void ReadCase(FILE *fp, IPTR pj, Population *p)
{
  int t, i;
  fscanf(fp, "%i", &t);
  /*   printf("Read time. \t lchrom = %d\n", lchrom);*/
  for(i = 0; i < p->chromLength; i++) {
    fscanf(fp, "%i", &(pj->chrom[i]));
  }
  fscanf(fp, "%lf %lf", &(pj->fitness), &(pj->scaledFitness));
}


int FindNCases(char *ncfile)
{
  FILE *fp;
  int tmp;

  if((fp = fopen(ncfile, "r")) == NULL){
    fprintf(stdout, "no cases in case base\n");
    return 0;
  } else {
    fscanf(fp, "%d", &tmp);
    fclose(fp);
    return tmp;
  }
}

void StoreNcases(char *ncfile, int ncases, int nCurrentCases)
{
  FILE *fp;

  if((fp = fopen(ncfile, "w")) == NULL){
    fprintf(stdout, "problem in opening %s \n", ncfile);
    exit(1);
  } else {
    fprintf(fp, "%d\n", ncases + nCurrentCases);
    fclose(fp);
  }
}

void InitLoadCases(char *caseFile, IPTR pop, int gen, int perc, Population *p)
{
  FILE *fp;
  int i;
  p->nCases = FindNCases(p->nCFile);
  if (p->nCases <= 0) return;
  caseBase = AllocateIndividuals(p->nCases, p->chromLength);
  if(caseBase == NULL){
    perror("Malloc failure for caseBase\n");
    exit(1);
  }
  if ((fp = fopen(caseFile, "r")) == NULL) {
    fprintf(stderr, "InitLoadCases: Cannot open %s for reading\n", caseFile);
    exit(1);
  } 
  for(i = 0; i < p->nCases; i++) {
    ReadCase(fp, &caseBase[i], p);
  }
  fclose(fp);
}
