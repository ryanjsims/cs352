#include <stdio.h>
#include <malloc.h>
#include <time.h>
#include "encapsulatedListStr.h"



EncList_Str *buildInputList(FILE*);
void         mergeSort_topDown(EncList_Str *list);
void         printList(EncList_Str *list);


int main()
{
	EncList_Str *strs = buildInputList(stdin);
	if (strs == NULL)
	{
		fprintf(stderr, "ERROR: Could not generate the input list.\n");
		return 1;
	}

	mergeSort_topDown(strs);
	printList(strs);

	encList_Str__free(strs);
	return 0;
}


EncList_Str *buildInputList(FILE *in)
{
	EncList_Str *list = encList_Str__alloc();
	if (list == NULL)
		return NULL;

	while (1)   // we'll break out manually, when we find EOF
	{
		char buf[256];
		int rc = scanf("%255s", buf);
		if (rc == -1)
			break;

		encList_Str__addHead(list, buf,1);
	}

	return list;
}


void mergeSort_topDown(EncList_Str *list)
{
	// we'll need to know the length of the list.  It's useful a couple of
	// times in this function.
	int len = encList_Str__count(list);

	if (len <= 1)
		return;    // NOP

	// break the list into two pieces, and then recurse.
	EncList_Str *right = encList_Str__splitAt(list, len/2);
	if (right == NULL)
		return;    // error case!

	// recurse to sort both lists
	mergeSort_topDown(list);
	mergeSort_topDown(right);

	// merge them together, then free the (now unused) right-hand list.
	encList_Str__merge(list, right);
	encList_Str__free (      right);

	// done!
	return;
}


void printList(EncList_Str *list)
{
	// print out the strings in the list
	EncNode_Str *cur = encList_Str__getHead(list);    // could be NULL

	while (cur != NULL)
	{
		printf("%s\n", encNode_Str__getStr(cur));
		cur = encNode_Str__getNext(cur);
	}

	return;
}


