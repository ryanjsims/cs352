#include "proj07_intList.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void quicksort(int *arr, int begin, int end);
int *partition(int *arr, int begin, int end);
void swap(int *arr, int pos1, int pos2);

IntList *readInput(FILE* inputFile){
	IntList* list = NULL;
	IntList* tail = NULL;
	int *values = malloc(10 * sizeof(int));
	if(values == NULL){
		perror("Could not allocate memory for intermediate array in readInput");
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

void quicksort(int *arr, int begin, int end){
	if(begin < end){
		int *p = partition(arr, begin, end);

		quicksort(arr, begin, p[0]);
		quicksort(arr, p[1], end);
		free(p);
	}
}

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

void swap(int *arr, int pos1, int pos2){
	int temp = arr[pos1];
	arr[pos1] = arr[pos2];
	arr[pos2] = temp;
}


int getLen(IntList* list){
	int size = 0;
	for(IntList *curr = list; curr != NULL; curr = curr->next)
		size++;
	return size;
}

void writeOutput(IntList* list, FILE* outputFile){
	int position = 0;
	for(IntList *curr = list; curr != NULL; curr = curr->next){
		fprintf(outputFile, "%d: %d\n", position, curr->val);
		position++;
	}
}

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
