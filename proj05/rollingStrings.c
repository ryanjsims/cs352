/*
 * File: rollingStrings.c
 * Author: Ryan Sims
 * Purpose: rollingStrings creates and fills a buffer of strings with strings read
 * from files given in command line arguments.
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
 * readOneStr(fp, err) -- returns a single string (or NULL if error occurs) 
 * read from fp. If an error occurs, prints error message to stderr and sets
 * the value at err equal to 1. The string is malloc'ed by readOneStr, and
 * must be freed when it is no longer used.
 * Assumes that fp and err are not NULL.
 */
char *readOneStr(FILE *fp, int *err);

/*
 * insertInOrder(toInsert, strings) -- inserts the string toInsert into the
 * buffer strings in the correct order determined by strcmp.
 * Assumes that strings is already sorted, that toInsert and strings are not NULL,
 * and that the length of strings is 16.
 */
void insertInOrder(char *toInsert, char **strings);

/*
 * printStrings(buffer) -- prints the strings in buffer to the screen.
 * Assumes that buffer is 16 strings long.
 */
void printStrings(char **buffer);

int main(int argc, char *argv[]){
	char *buffer[16];
	int error = 0;
	buffer[0] = "HEAD";
	buffer[15] = "TAIL";
	for(int i = 1; i < 15; i++){
		buffer[i] = NULL;
	}
	printf("There are %d command-line arguments\n", argc - 1);
	for(int i = 1; i < argc; i++){
		printf("--- FILE: %s (argv[%d]) ---\n", argv[i], i);
		FILE *currFile = fopen(argv[i], "r");
		if(currFile == NULL){
			fprintf(stderr, "ERROR: Cannot open the file '%s'\n", argv[i]);
			error = 1;
			continue;
		}
		char* newString;
		do {
			newString = readOneStr(currFile, &error);
			if(newString == NULL){
				printf("--- END OF FILE ---\n");
				break;
			}
			insertInOrder(newString, buffer);
			printStrings(buffer);
		} while(newString != NULL);
		fclose(currFile);
	}
	for(int j = 1; j < 15; j++){
		free(buffer[j]);
		buffer[j] = NULL;
	}
	return error;
}

/*
 * readOneStr(fp, err) -- returns a single string (or NULL if error occurs)
 * read from fp. If an error occurs, prints error message to stderr and sets
 * the value at err equal to 1. The string is malloc'ed by readOneStr, and
 * must be freed when it is no longer used.
 * Assumes that fp and err are not NULL.
 */
char *readOneStr(FILE *fp, int *err){
	int size, rc;
	do{
		rc = fscanf(fp, "%d", &size);
		if(rc == 0){
			char wrong;
			int chrc = fscanf(fp, "%c", &wrong);
			if(chrc < 1){
				return NULL;
			}
			printf("OOPS: Non-numeric input detected.  The byte was: 0x%02x='%c'.\n", wrong, wrong);
			*err = 1;
		} else if (rc == EOF){
			return NULL;
		} else if(size < 1){
			printf("OOPS: The length read from input, %d, was zero or negative.\n", size);
			*err = 1;
		}
	} while(rc == 0 || size < 1);
	char *string = (char*)malloc((size + 1) * sizeof(char));
	if(string == NULL){
		fprintf(stderr, "You ran out of memory. Wow. Great job.\n");
		free(string);
		*err = 1;
		return NULL;
	}
	char format[32];
	sprintf(format, "%%%ds", size);
	rc = fscanf(fp, format, string);
	if(rc == EOF){
		printf("OOPS: Hit EOF when trying to read a string of length %d.\n", size);
		*err = 1;
		free(string);
		return NULL;
	}
	return string;
}

/*
* insertInOrder(toInsert, strings) -- inserts the string toInsert into the
* buffer strings in the correct order determined by strcmp.
* Assumes that strings is already sorted, that toInsert and strings are not NULL,
* and that the length of strings is 16.
*/
void insertInOrder(char *toInsert, char **buffer){
	if(buffer[14] != NULL){
		free(buffer[1]);
		buffer[1] = toInsert;
	} else {
		char *prev = toInsert;
		char *curr;
		for(int i = 1; prev != NULL && i < 15; i++){
			curr = buffer[i];
			buffer[i] = prev;
			prev = curr;
		}
	}
	for(int i = 1; i < 14; i++){
		if(buffer[i + 1] == NULL)
			break;
		char* temp = buffer[i];
		if(strcmp(buffer[i], buffer[i+1]) > 0){
			buffer[i] = buffer[i+1];
			buffer[i+1] = temp;
		}
		else
			break;
	}
}

/*
 * printStrings(buffer) -- prints the strings in buffer to the screen.
 * Assumes that buffer is 16 strings long.
 */
void printStrings(char **buffer){
	printf("Current strings:\n");
	for(int i = 0; i < 16; i++){
		printf("  %d: ", i);
		if(buffer[i] == NULL){
			printf("<null>");
		} else {
			printf("%s", buffer[i]);
		}
		printf("\n");
	}
}
