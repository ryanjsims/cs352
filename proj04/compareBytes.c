#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

void dumpNext(char *filename, FILE *fp, unsigned char c);
void printChar(unsigned char c);
int compareFiles(char **names, FILE **ptrs);

int main(int argc, char** argv){
	if(argc != 3){
		fprintf(stderr, "Incorrect number of arguments.\n");
		return 1;
	}
	FILE **inputFiles = (FILE**)malloc(2 * sizeof(FILE*));
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
	fclose(*inputFiles);
	fclose(*(inputFiles + 1));
	return rc;
}

int compareFiles(char **names, FILE **ptrs){
	int readCharA, readCharB;
	int byteCount = 0;
	readCharA = fgetc(*ptrs);
	readCharB = fgetc(*(ptrs+1));
	while(readCharA != EOF && readCharB != EOF && readCharA == readCharB){
		readCharA = fgetc(*ptrs);
		readCharB = fgetc(*(ptrs+1));
		byteCount++;
	}
	if((readCharA == EOF || readCharB == EOF) && readCharA != readCharB){
		switch(readCharA){
			case EOF:
				printf("The file '%s' was shorter than '%s'.\n", *names, *(names + 1));
				dumpNext(*(names + 1), *(ptrs + 1), (unsigned char)readCharB);
				break;
			default:
				printf("The file '%s' was shorter than '%s'.\n", *(names + 1), *names);
				dumpNext(*names, *ptrs, (unsigned char)readCharA);
				break;
		}
		return 1;
	} else if(readCharA != readCharB) {
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

void dumpNext(char *filename, FILE *fp, unsigned char c){
	long currPos = ftell(fp), endPos;
	int loopNum = 4, charAsInt;
	fseek(fp, 0l, SEEK_END);
	endPos = ftell(fp);
	fseek(fp, currPos, SEEK_SET);
	if(endPos - currPos < 4){
		loopNum = (int)(endPos - currPos);
		printf("The file '%s' only has %ld bytes left:", filename, endPos - currPos);
	} else {
		printf("The next 4 bytes of '%s' are:", filename);
	}
	for(int i = 0; i < loopNum; i++){
		printChar(c);
		charAsInt = fgetc(fp);
		c = (unsigned char)charAsInt;
		if(charAsInt != EOF && i < loopNum - 1)
			printf(" ");
	}
	printf("\n");
}

void printChar(unsigned char c){
	printf("0x%02x=", c);
	if(c == '\0')
		printf("<null>");
	else if(!isprint(c))
		printf("<unprintable>");
	else
		printf("'%c'", c);
}