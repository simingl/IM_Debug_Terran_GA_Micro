#pragma once

#include <stdio.h>
#include "type.h"
#include "utils.h"

int FindCity(ChromType city, ChromType *tour, int lchrom);
void SwapMutate(ChromType *ci1, ChromType * ci2, int lchrom, float pMut);

void PMX(IPTR p1, IPTR p2, IPTR c1, IPTR c2, Population *p);
void Copy(ChromType *child, ChromType *parent, int lchromx);
void SwapCity(ChromType *, ChromType *, int lchrom);
int Flip(double);
int Rnd(int, int);

/* chromosome type must be int for sequencing problems */
void Copy(ChromType *child, ChromType *parent, int lchrom)
{
  int i;
  for(i = 0; i < lchrom; i++){
    child[i] = parent[i];
  }
}

void PMX(IPTR p1, IPTR p2, IPTR c1, IPTR c2, Population *p)
{
  int  i;
  int xp1, xp2, fcity;

  /*  fprintf(stdout, "In PMX \n"); */

  Copy(c1->chrom, p1->chrom, p->chromLength);
  /*  fprintf(stdout, "C1\n");; */
  /*  PrintChrom(c1); */
  /*  PrintChrom(p1); */
  /*  Copy(c1->JSSPchrom, p1->JSSPchrom); */
  Copy(c2->chrom, p2->chrom, p->chromLength);
  /*  Copy(c2->JSSPchrom, p2->JSSPchrom); */

  /*  fprintf(stdout, "C2\n"); */
  /*  PrintChrom(c2); */
  /*  PrintChrom(p2); */

  if (!Flip(p->pCross)) {
    return;
  }

  /*   ncross++; */
  xp1 = Rnd(0, p->chromLength - 1); /* remember that rnd returns a number between */
  xp2 = Rnd(0, p->chromLength - 1); /* low and high inclusive */

  if(xp2 < xp1) SwapInt(&xp1, &xp2);

  for(i = xp1; i <= xp2; i++) {
    /*<= since both are guaranteed to be in range */

    fcity = FindCity(p1->chrom[i], p2->chrom, p->chromLength); 
    /* making c2 */
    SwapChromType(& c2->chrom[fcity], &c2->chrom[i]); 
    /* SwapInt aka SwapCity */

    fcity = FindCity(p2->chrom[i], p1->chrom, p->chromLength); 
    /* making c1 */
    SwapChromType(&c1->chrom[fcity], & c1->chrom[i]);
  }
  
  /* need to do mutation seperately here */
  SwapMutate(c1->chrom, c2->chrom, p->chromLength, p->pMut);

}

void SwapMutate(ChromType *ci1, ChromType * ci2, int lchrom, float pMut)
{
  int i;
  for(i = 0; i < lchrom; i++){
    if(Flip(pMut)){
      SwapChromType(&ci1[Rnd(0, lchrom-1)], &ci1[Rnd(0, lchrom-1)]);
    }
    if(Flip(pMut)){
      SwapChromType(&ci2[Rnd(0, lchrom-1)], & ci2[Rnd(0, lchrom-1)]);
    }
  }
}


int FindCity(ChromType city, ChromType *tour, int lchrom)
{
  int i;
  for(i = 0; i < lchrom; i++) {
    if(city == tour[i]) return i;
  }
  fprintf(stderr, "PMX: Couldn't find city %d\n", city);
  /*  PrintTour(tour, stderr); */
  exit(1);
}




