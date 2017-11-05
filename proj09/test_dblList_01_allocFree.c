
/* Test the alloc() and free() methods of the DblList_Int "class".  Does
 * various combinations of the operations, in various orders, to see
 * what happens.
 *
 * This file looks inside the fields of the dblList objects, and ensures
 * that they are exactly as required.  This means that it can skip the
 * gettors; they don't have to be implemented yet.
 */

#include <stdio.h>
#include <stdlib.h>

#include "dblListInt.h"



/* this allocates and frees lots of data, and fills it with random
 * values, to check for uninitialized values later on, after other
 * malloc() operations.
 *
 * Is this overkill?  Maybe we should just let valgrind find these
 * bugs?
 */
void trashMallocBuffers();


/* helper functions for common tests */
void inspect(DblList_Int*, int expectedVal);


int main()
{
	trashMallocBuffers();

	DblList_Int *nodeA = dblList_Int__alloc(0);
	DblList_Int *nodeB = dblList_Int__alloc(1000);
	DblList_Int *nodeC = dblList_Int__alloc(-1);
	DblList_Int *nodeD = dblList_Int__alloc(17);

	inspect(nodeA, 0);
	inspect(nodeB, 1000);
	inspect(nodeC, -1);
	inspect(nodeD, 17);

	dblList_Int__free(nodeB);
	nodeB = dblList_Int__alloc(123);

	inspect(nodeA, 0);
	inspect(nodeB, 123);
	inspect(nodeC, -1);
	inspect(nodeD, 17);

	dblList_Int__free(nodeA);
	dblList_Int__free(nodeB);
	dblList_Int__free(nodeC);
	dblList_Int__free(nodeD);

	return 0;
}



void trashMallocBuffers()
{
	for (int size = sizeof(DblList_Int)/4;
	     size    <= sizeof(DblList_Int)*8;
	     size *= 2)
	{
		for (int i=0; i<1024; i++)
		{
			char *buf = malloc(size);
			if (buf == NULL)
			{
				perror("trashMallocBuffers(): malloc failed");
				continue;
			}

			for (int pos=0; pos<size; pos++)
				buf[pos] = rand() % 256;

			free(buf);
		}
	}
}



void inspect(DblList_Int *node, int expectedVal)
{
	if (node == NULL)
	{
		printf("inspect() found an error: node=NULL\n");
		return;
	}

	int err = 0;
	if (node->val != expectedVal)
	{
		printf("inspect() found an error: expectedVal=%d node->val=%d\n",
		       expectedVal, node->val);
		err = 1;
	}

	if (node->next != NULL || node->prev != NULL)
	{
		printf("inspect() found an error: next or prev is non-NULL: node->next=%p node->prev=%p\n", node->next, node->prev);
		err = 1;
	}

	if (err == 0)
		printf("inspect(): Test passed on one object, val=%d\n", node->val);
}


