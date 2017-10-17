#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Record{
	int firstInt, secondInt;
	char word[16];
} Record;

void printRecord(Record *rec);
int fillRecord(Record *buf, int index);
int checkForEnd();

int main(int argc, char **argv){
	int size = 1;
	Record *buffer = malloc(sizeof(Record));
	int rc = 0, i = 0;
	do{
		if(i == size){
			printf("Need to extend the array; the buffer is full (with %d records)\n", size);
			printf("  Before extending the array, element 0 is: ");
			printRecord(buffer);
			size = 2 * size;
			Record *new = malloc(size * sizeof(Record));
			memcpy(new, buffer, (size / 2) * sizeof(Record));
			free(buffer);
			buffer = new;
			printf("  After  extending the array, element 0 is: ");
			printRecord(buffer);
		}
		rc = fillRecord(buffer, i);
		if(rc > 0)
			i++;
		if(rc == 0 && !checkForEnd()){
			//Format error
			free(buffer);
			return 1;
		} else if(rc < 0){
			//Format Error
			free(buffer);
			return 1;
		}
	} while(rc > 0);
	
	int index = -1;
	rc = scanf("%d", &index);
	while(rc > 0){
		printf("[%d] ", index); 
		if(index >= 0 && index < i){
			printRecord(buffer + index);
		} else {
			printf("<invalid index>\n");
		}
		rc = scanf("%d", &index);
	}
	if(rc == 0){
		fprintf(stderr, "ERROR: scanf() rc=0\n");
		free(buffer);
		return 1;
	}
	free(buffer);
	return 0;

}

void printRecord(Record *rec){
	 printf("%d %d %s\n", rec->firstInt, rec->secondInt, rec->word);
}

int fillRecord(Record *buf, int index){
	int rc = scanf("%d %d %15s", &buf[index].firstInt, &buf[index].secondInt, buf[index].word);
	if(rc != 3 && rc != 0){
		fprintf(stderr, "ERROR: scanf() rc=%d", rc);
		return -1;
	}
	return rc;
}

int checkForEnd(){
	printf("scanf() returned 0, so now we'll check to see if the string is \"END\"...\n");
	char temp[4];
	int rc = scanf("%3s", temp);
	if(rc > 0 && strcmp(temp, "END") == 0){
		printf("...\"END\" found.  Will now read the indices...\n");
		return 1;
	} else {
		fprintf(stderr, "ERROR: the string was not \"END\".  Ending the program!\n");
		return 0;
	}
}
