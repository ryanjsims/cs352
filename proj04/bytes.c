#include <stdio.h>

void dumpNext(char *filename, FILE *fp, unsigned char c);
void printChar(unsigned char c);
void compareFiles(char **names, FILE **ptrs);

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
	compareFiles(argv + 1, inputFiles);
	fclose(*inputFiles);
	fclose(*(inputFiles + 1));
	return 0;
}

void compareFiles(char **names, FILE **ptrs){
	unsigned char readCharA, readCharB;
	int byteCount = 0;
	readCharA = (unsigned char)fgetc(*ptrs);
	readCharB = (unsigned char)fgetc(*(ptrs+1));
	while(readCharA != EOF && readCharB != EOF && readCharA == readCharB){
		readCharA = (unsigned char)fgetc(*ptrs);
		readCharB = (unsigned char)fgetc(*(ptrs+1));
		byteCount++;
	}
	if(readCharA == EOF || readCharB == EOF){
		switch(readCharA){
			case EOF:
				printf("The file '%s' was shorter than '%s'.\n", *names, *(names + 1));
				dumpNext(*(names + 1), *(ptrs + 1), readCharB);
				break;
			default:
				printf("The file '%s' was shorter than '%s'.\n", *(names + 1), *names);
				dumpNext(*names, *ptrs, readCharA);
				break;
		}
	} else if(readCharA != readCharB) {
		printf("Difference found at byte %d\n", byteCount);
		dumpNext(*names, *ptrs, readCharA);
		dumpNext(*(names + 1), *(ptrs + 1), readCharB);
	} else {
		//Hooray, the files match, congratulations, you won a car and 5 million dollarydoos!
		printf("No differences found.\n");
	}
}

void dumpNext(char *filename, FILE *fp, unsigned char c){
	long currPos = ftell(fp), endPos;
	int loopNum = 4;
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
		c = (unsigned char)fgetc(fp);
	}
	printf("\n");
}

void printChar(unsigned char c)Hello
