/*
* intSort_funcs.c
* Author: Ryan Sims
* Purpose: Implement the functions necessary for strSort to read input from file,
* sort the data, create a linkedlist from the data, and write data as output to file
*/

#include "proj07_strList.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/*
 * quicksort(char** arr, int begin, int end)
 * Performs a recursive quicksort on the array stored in arr between and including
 * the indices begin and end. Uses partition helper function.
 */
void quicksort(char **arr, int begin, int end);

/*
 * partition(char** arr, int begin, int end)
 * Partitions the array in arr between begin and end using a pivot selected at (begin + end) / 2
 * Creates 3 partitions: strcmp(strs, pivot) < 0, strcmp(strs, pivot) == 0, and strcmp(strs, pivot) > 0.
 * Returns an int array of length 2 where array[0] is the left index of the middle partition,
 * and array[1] is the right index of the middle partition.
 */
int *partition(char **arr, int begin, int end);

/*
 * swap(char** arr, int pos1, int pos2)
 * Shockingly, this function swaps two items in the arr at positions pos1 and pos2.
 */
void swap(char **arr, int pos1, int pos2);

/*
 * readInput(FILE* inputFile)
 * reads string input from inputFile into a dynamically expanded
 * array, quicksorts the array, and then creates a LinkedList
 * from the values in the array, using a tail pointer to avoid
 * traversing the LinkedList.
 * On read error or malloc error: writes message to stderr and returns
 * whatever input has been read so far. If no input read, returns
 * NULL.
 * Returns an StrList containing as much data as could be read from
 * the file.
 */
StrList *readInput(FILE* inputFile){
	StrList* list = NULL;
	StrList* tail = NULL;
	char **values = malloc(10 * sizeof(char*));
	if(values == NULL){
		perror("Could not allocate memory for intermediate array in readInput");
		return NULL;
	}

	int size = 0, capacity = 10;
	do{
		char *toAdd = readLongStr(inputFile);
		if(strcmp(toAdd, "") == 0 || strcmp(toAdd, "(null)") == 0){
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

/*
 * quicksort(char** arr, int begin, int end)
 * Performs a recursive quicksort on the array stored in arr between and including
 * the indices begin and end. Uses partition helper function.
 */
void quicksort(char **arr, int begin, int end){
	if(begin < end){
		int *p = partition(arr, begin, end);

		quicksort(arr, begin, p[0]);
		quicksort(arr, p[1], end);
		free(p);
	}
}

/*
 * partition(char** arr, int begin, int end)
 * Partitions the array in arr between begin and end using a pivot selected at (begin + end) / 2
 * Creates 3 partitions: strcmp(strs, pivot) < 0, strcmp(strs, pivot) == 0, and strcmp(strs, pivot) > 0.
 * Returns an int array of length 2 where array[0] is the left index of the middle partition,
 * and array[1] is the right index of the middle partition.
 */
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

/*
 * swap(char** arr, int pos1, int pos2)
 * Shockingly, this function swaps two items in the arr at positions pos1 and pos2.
 */
void swap(char **arr, int pos1, int pos2){
	char *temp = arr[pos1];
	arr[pos1] = arr[pos2];
	arr[pos2] = temp;
}

/*
 * readLongStr(FILE* in)
 * Reads one string delineated by whitespace from the file.
 * Ignores and discards any leading whitespace.
 * Returns the string on success.
 * If no input can be read, returns "(null)"
 */
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
	if(strcmp(str, "") == 0)
		strcpy(str, "(null)");
	return str;
}

/*
 * getLen(StrList* list)
 * returns the length of the list
 */
int getLen(StrList* list){
	int size = 0;
	for(StrList *curr = list; curr != NULL; curr = curr->next)
		size++;
	return size;
}

/*
 * writeOutput(StrList* list, FILE* outputFile)
 * Writes the contents of list to outputFile in the format <index>: <val>\n
 */
void writeOutput(StrList* list, FILE* outputFile){
	int position = 0;
	for(StrList *curr = list; curr != NULL; curr = curr->next){
		fprintf(outputFile, "%d: %s\n", position, curr->val);
		position++;
	}
}

/*
 * freeList(IntList* list)
 * Frees the given linkedlist and all of its values.
 */
void freeList(StrList* list){
	if(list == NULL)
		return;
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
