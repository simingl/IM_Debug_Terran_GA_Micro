#pragma once

#include <math.h>
#include "type.h"
#include "dist.h"

double HamDist(ChromType *p1, ChromType *p2, int size)
{
/*  find the hamming ditance between two binary strings */
  int dist,i;

  dist = 0;
  for(i = 0; i < size; i++) {
    dist += (p1[i] == p2[i] ? 0 : 1);
  }
  return (double) dist;
}

double Euclidean(ChromType *p1, ChromType *p2, int size)
{
  double dist, tmp;
  int i;

  dist = 0.0;
  for(i = 0; i < size; i++){
    tmp =  (p1[i] - p2[i]);
    dist += tmp* tmp;
  }
  return sqrt((double) dist);
}

double LCS(ChromType *p1, ChromType *p2, int size)
{
  return 0.0;
}
