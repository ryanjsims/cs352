#ifndef __PROJ07_INTLIST_H__INCLUDED__
#define __PROJ07_INTLIST_H__INCLUDED__
#include <stdio.h>

typedef struct IntList IntList;
struct IntList
{
	int      val;
	IntList *next;
};


/* ---- YOU MUST IMPLEMENT THESE FUNCTIONS ---- */

IntList *readInput  (         FILE*);
int      getLen     (IntList*);
void     writeOutput(IntList*,FILE*);
void     freeList   (IntList*);


#endif

