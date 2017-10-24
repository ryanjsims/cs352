#include "proj07_strList.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

void quicksort(char **arr, int begin, int end);
int *partition(char **arr, int begin, int end);
void swap(char **arr, int pos1, int pos2);

StrList *readInput(FILE* inputFile){
	StrList* list = NULL;
	StrList* tail = NULL;
	char **values = malloc(10 * sizeof(char*));
	if(values == NULL){
		perror("Could not allocate memory for intermediate array in readInput");
	}

	int size = 0, capacity = 10;
	do{
		char *toAdd = readLongStr(inputFile);
		if(strcmp(toAdd, "") == 0){
			free(toAdd);
			continue;
		}
		values[size] = toAdd;
		size++;
		if(size == capacity){
			char **expVals = malloc(capacity * 2 * sizeof(char*));
			if(expVals == NULL){
				perror("Malloc failed in readInput");
				break;
			}
			memcpy(expVals, values, capacity * sizeof(char*));
			capacity *= 2;
			free(values);
			values = expVals;
		}
	} while(!feof(inputFile));
	quicksort(values, 0, size - 1);
	for(int i = 0; i < size; i++){
		StrList* new = malloc(sizeof(StrList));
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

void quicksort(char **arr, int begin, int end){
	if(begin < end){
		int *p = partition(arr, begin, end);

		quicksort(arr, begin, p[0]);
		quicksort(arr, p[1], end);
		free(p);
	}
}

int *partition(char **arr, int begin, int end){
	int pivotLoc = (begin + end) / 2;
	char *pivot = arr[pivotLoc];
	int i = begin, j = end;
	int *toReturn = malloc(2 * sizeof(int));
	while(1){
		while(strcmp(arr[i], pivot) < 0)
			i++;
		while(strcmp(arr[j], pivot) > 0)
			j--;
		if(i >= j){
			while(i >= 0 && strcmp(arr[i], pivot) == 0)
				i--;
			while(j <= end && strcmp(arr[j], pivot) == 0)
				j++;
			toReturn[0] = i;
			toReturn[1] = j;
			return toReturn;
		}
		swap(arr, i, j);
		if(strcmp(arr[i], arr[j]) == 0)
			j--;
	}
}

void swap(char **arr, int pos1, int pos2){
	char *temp = arr[pos1];
	arr[pos1] = arr[pos2];
	arr[pos2] = temp;
}

char *readLongStr(FILE* in){
	char *str = malloc(8 * sizeof(char));
	int size = 0, capacity = 8;
	char curr = (char)fgetc(in);
	while(isspace(curr))
		curr = (char)fgetc(in);

	while(curr != EOF && !isspace(curr)){
		str[size] = curr;
		size++;
		curr = (char)fgetc(in);
		if(size == capacity){
			char *newStr = malloc(2 * capacity * sizeof(char));
			if(newStr == NULL){
				perror("Failed to malloc() in readLongStr");
				break;
			}
			memcpy(newStr, str, capacity);
			capacity *= 2;
			free(str);
			str = newStr;
		}

	} 
	str[size] = '\0';
	return str;
}


int getLen(StrList* list){
	int size = 0;
	for(StrList *curr = list; curr != NULL; curr = curr->next)
		size++;
	return size;
}

void writeOutput(StrList* list, FILE* outputFile){
	int position = 0;
	for(StrList *curr = list; curr != NULL; curr = curr->next){
		fprintf(outputFile, "%d: %s\n", position, curr->val);
		position++;
	}
}

void freeList(StrList* list){
	StrList *prev = list;
	StrList *curr = list->next;
	while(prev != NULL){
		free(prev->val);
		free(prev);
		prev = curr;
		if(curr != NULL)
			curr = curr->next;
	}
}
