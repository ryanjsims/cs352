/*
 * intSort_funcs.c
 * Author: Ryan Sims
 * Purpose: Implement the functions necessary for intSort to read input from file,
 * sort the data, create a linkedlist from the data, and write data as output to file
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "proj07_intList.h"

/*
 * quicksort(int* arr, int begin, int end)
 * Performs a recursive quicksort on the array stored in arr between and including
 * the indices begin and end. Uses partition helper function.
 */
void quicksort(int *arr, int begin, int end);

/*
 * partition(int* arr, int begin, int end)
 * Partitions the array in arr between begin and end using a pivot selected at (begin + end) / 2
 * Creates 3 partitions: nums < pivot, nums == pivot, and nums > pivot.
 * Returns an int array of length 2 where array[0] is the left index of the middle partition,
 * and array[1] is the right index of the middle partition.
 */
int *partition(int *arr, int begin, int end);

/*
 * swap(int* arr, int pos1, int pos2)
 * Shockingly, this function swaps two items in the arr at positions pos1 and pos2.
 */
void swap(int *arr, int pos1, int pos2);

/*
 * readInput(FILE* inputFile)
 * reads integer input from inputFile into a dynamically expanded
 * array, quicksorts the array, and then creates a LinkedList
 * from the values in the array, using a tail pointer to avoid
 * traversing the LinkedList.
 * On read error or malloc error: writes message to stderr and returns
 * whatever input has been read so far. If no input read, returns
 * NULL.
 * Returns an IntList containing as much data as could be read from
 * the file.
 */
IntList *readInput(FILE* inputFile){
	IntList* list = NULL;
	IntList* tail = NULL;
	int *values = malloc(10 * sizeof(int));
	if(values == NULL){
		perror("Could not allocate memory for intermediate array in readInput");
		return NULL;
	}

	int size = 0, capacity = 10;
	int rc;
	do{
		rc = fscanf(inputFile, "%d", values + size);
		if(rc > 0)
			size++;
		if(size == capacity){
			int *expVals = malloc(capacity * 2 * sizeof(int));
			if(expVals == NULL){
				perror("Malloc failed in readInput");
				break;
			}
			memcpy(expVals, values, capacity * sizeof(int));
			capacity *= 2;
			free(values);
			values = expVals;
		}
		if(rc == 0){
			perror("Non-numeric input encountered");
		}
	} while(rc > 0);

	quicksort(values, 0, size - 1);

	for(int i = 0; i < size; i++){
		IntList* new = malloc(sizeof(IntList));
		if(new == NULL){
			perror("Could not allocate memory for linkedlist in readInput");
			break;
		}
		new->next = NULL;
		new->val = values[i];
		if(list == NULL){
			list = new;
			tail = list;
		} else {
			tail->next = new;
			tail = tail->next;
		}
	}
	free(values);
	return list;
}

/*
 * quicksort(int* arr, int begin, int end)
 * Performs a recursive quicksort on the array stored in arr between and including
 * the indices begin and end. Uses partition helper function.
 */
void quicksort(int *arr, int begin, int end){
	if(begin < end){
		int *p = partition(arr, begin, end);

		quicksort(arr, begin, p[0]);
		quicksort(arr, p[1], end);
		free(p);
	}
}

/*
 * partition(int* arr, int begin, int end)
 * Partitions the array in arr between begin and end using a pivot selected at (begin + end) / 2
 * Creates 3 partitions: nums < pivot, nums == pivot, and nums > pivot.
 * Returns an int array of length 2 where array[0] is the left index of the middle partition,
 * and array[1] is the right index of the middle partition.
 */
int *partition(int *arr, int begin, int end){
	int pivotLoc = (begin + end) / 2;
	int pivot = arr[pivotLoc];
	int i = begin, j = end;
	int *toReturn = malloc(2 * sizeof(int));
	while(1){
		while(arr[i] < pivot)
			i++;
		while(arr[j] > pivot)
			j--;
		if(i >= j){
			while(i >= 0 && arr[i] == pivot)
				i--;
			while(j <= end && arr[j] == pivot)
				j++;
			toReturn[0] = i;
			toReturn[1] = j;
			return toReturn;
		}
		swap(arr, i, j);
		if(arr[i] == arr[j])
			j--;
	}
}

/*
 * swap(int* arr, int pos1, int pos2)
 * Shockingly, this function swaps two items in the arr at positions pos1 and pos2.
 */
void swap(int *arr, int pos1, int pos2){
	int temp = arr[pos1];
	arr[pos1] = arr[pos2];
	arr[pos2] = temp;
}

/*
 * getLen(IntList* list)
 * returns the length of the list
 */
int getLen(IntList* list){
	int size = 0;
	for(IntList *curr = list; curr != NULL; curr = curr->next)
		size++;
	return size;
}

/*
 * writeOutput(IntList* list, FILE* outputFile)
 * Writes the contents of list to outputFile in the format <index>: <val>\n
 */
void writeOutput(IntList* list, FILE* outputFile){
	int position = 0;
	for(IntList *curr = list; curr != NULL; curr = curr->next){
		fprintf(outputFile, "%d: %d\n", position, curr->val);
		position++;
	}
}

/*
 * freeList(IntList* list)
 * Frees the given linkedlist.
 */
void freeList(IntList* list){
	if(list == NULL)
		return;
	IntList *prev = list;
	IntList *curr = list->next;
	while(prev != NULL){
		free(prev);
		prev = curr;
		if(curr != NULL)
			curr = curr->next;
	}
}
