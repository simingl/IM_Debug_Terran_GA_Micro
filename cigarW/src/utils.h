
#ifndef _UTILS_H
#define _UTILS_H


void   strcreate(char **dest, char *src);
void   syserror(char *loc);
void   error(char *loc, char *message);
int    Flip(double prob);
void   SwapInt(int *, int *);
void   SwapChromType(ChromType *, ChromType *);
int    AddModulo(int lim, int x, int y);
void   skipline(FILE *fp);
void   Shuffle(int *deck, int size);
void   IndividualCopy(individual *from, individual *to);

IPTR AllocateIndividuals(int howmany, int lchrom);

void DecToBin(int val, int size, ChromType *bits);
void CopyBits(ChromType *bits, ChromType *gene, int size);

#endif /* _UTILS_H */
