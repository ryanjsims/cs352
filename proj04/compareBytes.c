/* compareBytes.c
 * Programmer: Ryan Sims
 * Date: 9/25/2017
 *
 * compareBytes compares files byte by byte until it finds a difference, after which it
 * prints the next 4 bytes of each file (or however many remain)
 * Returns 1 on any error or if a difference is found. Returns 0 if both files are identical
 */

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

void dumpNext(char *filename, FILE *fp, unsigned char c);
void printChar(unsigned char c);
int compareFiles(char **names, FILE **ptrs);

int main(int argc, char** argv){
	//Checks for correct number of args
	if(argc != 3){
		fprintf(stderr, "Incorrect number of arguments.\n");
		return 1;
	}

	//Allocate space for 2 files, open them and check for errors
	FILE **inputFiles = (FILE**)malloc(2 * sizeof(FILE*));
	if (inputFiles == NULL) {
		fprintf(stderr, "Could not allocate space for input files.\n");
		return 1;
	}
	*inputFiles = NULL;
	*(inputFiles + 1) = NULL;
	*inputFiles = fopen(*(argv + 1), "r");
	if(*inputFiles == NULL){
		fprintf(stderr, "Unable to open file %s.\n", *(argv + 1));
		return 1;
	}
	*(inputFiles + 1) = fopen(*(argv + 2), "r");
	if(*(inputFiles + 1) == NULL){
		fprintf(stderr, "Unable to open file %s.\n", *(argv + 2));
		return 1;
	}

	int rc = compareFiles(argv + 1, inputFiles);
	
	//Close files and free allocated memory
	fclose(*inputFiles);
	fclose(*(inputFiles + 1));
	free(inputFiles);
	return rc;
}

/*
 * compareFiles does the bulk of the comparison work.
 * It reads characters from each file until either one file ends,
 * or the characters do not match, and calls dumpNext as necessary.
 * Returns 1 on difference, 0 on exact match.
 */
int compareFiles(char **names, FILE **ptrs){
	int readCharA, readCharB;
	int byteCount = 0;
	readCharA = fgetc(*ptrs);
	readCharB = fgetc(*(ptrs+1));
	//Read until end found or chars are different
	while(readCharA != EOF && readCharB != EOF && readCharA == readCharB){
		readCharA = fgetc(*ptrs);
		readCharB = fgetc(*(ptrs+1));
		byteCount++;
	}
	//Check if the end was found on only one file
	if((readCharA == EOF || readCharB == EOF) && readCharA != readCharB){
		//Assume second file longer
		char *shortFileName = *names, *longFileName = *(names + 1);
		FILE *longFP = *(ptrs + 1);
		
		//If wrong, change values to first file
		if (readCharA != EOF) {
			shortFileName = *(names + 1);
			longFileName = *names;
			longFP = *ptrs;
		}
		printf("The file '%s' was shorter than '%s'.\n", shortFileName, longFileName);
		dumpNext(longFileName, longFP, (unsigned char)readCharB);
		return 1;
	} else if(readCharA != readCharB) {
		//Found difference, print next bytes of both files
		printf("Difference found at byte %d.\n", byteCount);
		dumpNext(*names, *ptrs, (unsigned char)readCharA);
		dumpNext(*(names + 1), *(ptrs + 1), (unsigned char)readCharB);
		return 1;
	} else {
		//Hooray, the files match, congratulations, you won a car and 5 million dollarydoos!
		printf("No differences found.\n");
		return 0;
	}
}

/*
 * dumpNext dumps (up to) the next 4 bytes of a file.
 * 
 */
void dumpNext(char *filename, FILE *fp, unsigned char c){
	//Store current position of cursor in fp
	long currPos = ftell(fp);
	long endPos;
	int loopNum = 4;
	int charAsInt;
	//Go to end of file and store end position in endPos
	fseek(fp, 0l, SEEK_END);
	endPos = ftell(fp);
	
	//Go back to correct location
	fseek(fp, currPos, SEEK_SET);

	//Check to see if there are less than 4 bytes remaining to print message accordingly.
	if(endPos - currPos < 4){
		loopNum = (int)(endPos - currPos);
		printf("The file '%s' only has %ld bytes left:", filename, endPos - currPos);
	} else {
		printf("The next 4 bytes of '%s' are:", filename);
	}
	//Print the next chars with printChar
	for(int i = 0; i < loopNum; i++){
		printChar(c);
		charAsInt = fgetc(fp);
		c = (unsigned char)charAsInt;
		if(charAsInt != EOF && i < loopNum - 1)
			printf(" ");
	}
	printf("\n");
}

/*
 * printChar prints a single character in the format:
 *    0xXX='<val>'
 * after checking if the character is null or unprintable
 */
void printChar(unsigned char c){
	printf("0x%02x=", c);
	if(c == '\0')
		printf("<null>");
	else if(!isprint(c))
		printf("<unprintable>");
	else
		printf("'%c'", c);
}
