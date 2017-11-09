
/* Test the addAfter() function, which is useful for building short
 * linked lists.
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
void inspect(DblList_Int*, int expectedVal,
             DblList_Int *prev, DblList_Int *next);


int main()
{
	trashMallocBuffers();

	DblList_Int *nodeA = dblList_Int__alloc(10);
	DblList_Int *nodeB = dblList_Int__alloc(20);
	DblList_Int *nodeC = dblList_Int__alloc( 0);
	DblList_Int *nodeD = dblList_Int__alloc( 5);

	inspect(nodeA, 10, NULL,NULL);
	inspect(nodeB, 20, NULL,NULL);
	inspect(nodeC,  0, NULL,NULL);
	inspect(nodeD,  5, NULL,NULL);

	printf("Chaining the objects together...\n");
	dblList_Int__addAfter(nodeB, nodeC);
	dblList_Int__addAfter(nodeB, nodeA);
	dblList_Int__addAfter(nodeC, nodeD);

	inspect(nodeB, 20, NULL,  nodeA);
	inspect(nodeA, 10, nodeB, nodeC);
	inspect(nodeC,  0, nodeA, nodeD);
	inspect(nodeD,  5, nodeC, NULL);

	printf("Swapping A and C...\n");
	dblList_Int__swapWithNext(nodeA);
	
	inspect(nodeB, 20, NULL,  nodeC);
	inspect(nodeC,  0, nodeB, nodeA);
	inspect(nodeA, 10, nodeC, nodeD);
	inspect(nodeD,  5, nodeA, NULL);

	printf("Swapping A and C back...\n");
	dblList_Int__swapWithNext(nodeC);
	
	inspect(nodeB, 20, NULL,  nodeA);
	inspect(nodeA, 10, nodeB, nodeC);
	inspect(nodeC,  0, nodeA, nodeD);
	inspect(nodeD,  5, nodeC, NULL);
	
	printf("Removing 20 from the list...\n");
	dblList_Int__remove(nodeB);

	inspect(nodeB, 20, NULL,  NULL);
	inspect(nodeA, 10, NULL,  nodeC);
	inspect(nodeC,  0, nodeA, nodeD);
	inspect(nodeD,  5, nodeC, NULL);

	printf("Removing 5 from the list...\n");
	dblList_Int__remove(nodeD);

	inspect(nodeB, 20, NULL,  NULL);
	inspect(nodeA, 10, NULL,  nodeC);
	inspect(nodeC,  0, nodeA, NULL);
	inspect(nodeD,  5, NULL,  NULL);

	printf("Removing 0 from the list...\n");
	dblList_Int__remove(nodeC);

	inspect(nodeB, 20, NULL, NULL);
	inspect(nodeA, 10, NULL, NULL);
	inspect(nodeC,  0, NULL, NULL);
	inspect(nodeD,  5, NULL, NULL);

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



void inspect(DblList_Int *node, int expectedVal,
             DblList_Int *prev, DblList_Int *next)
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

	if (node->next != next || node->prev != prev)
	{
		printf("inspect() found an error: next or prev mismatch on the node with value %d\n", node->val);
		err = 1;
	}

	if (err == 0 && prev != NULL && node->prev->next != node)
	{
		printf("inspect(): node->prev->next does not point back at node.  node->val=%d\n", node->val);
		err = 1;
	}

	if (err == 0 && next != NULL && node->next->prev != node)
	{
		printf("inspect(): node->next->prev does not point back at node.  node->val=%d\n", node->val);
		err = 1;
	}

	if (err == 0)
	{
		printf("inspect(): Test passed on one object, val=%d :", node->val);

		if (node->prev == NULL)
			printf(" prev=<null>");
		else
			printf(" prev->val=%d", node->prev->val);

		if (node->next == NULL)
			printf(" next=<null>");
		else
			printf(" next->val=%d", node->next->val);

		printf("\n");
	}


	// segfaults happen often in early version of the code, get the
	// most debugging information out that we can.
	fflush(NULL);
}


