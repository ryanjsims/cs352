#include <stdio.h>
#include <stdlib.h>


void dumpInts(int *array, int count, int hex);
void insertInOrder(int *array, int count, int insert);

int main(){
	int size, toAdd = 0;
	char err = 7;
	int *buffer;
	int rc = scanf("%d", &size);
	int ret = 0;
	
	if(rc <= 0) {
		fprintf(stderr, "Could not read size of buffer!\n");
		return 1;
	} else if(size < 1) {
		fprintf(stderr, "Size was less than 1!\n");
		return 1;
	}

	buffer = (int*)malloc((size + 2) * sizeof(int));
	if(buffer == NULL){
		fprintf(stderr, "Buffer could not be allocated.\n");
		return 1;
	}

	*buffer = 0xdeadbeef;
	*(buffer + size + 1) = 0xc0d4f00d;
	for(int i = 1; i <= size; i++){
		*(buffer + i) = i;
	}

	while(rc >= 0){
		if(rc == 0){
			printf("OOPS: Non-integer input!  ");
			rc = scanf("%c", &err);
			printf("0x%02x='%c'\n", err, err);
			ret = 1;
		} else {
			if(err != 7)
				insertInOrder(buffer, size, toAdd);
			dumpInts(buffer, size + 1, 0);
			dumpInts(buffer, size + 1, 1);
			err = 'b';
		}
		rc = scanf("%d", &toAdd);
		if(rc >= 0)
			printf("\n");
	}
	return ret;
}

void dumpInts(int *array, int count, int hex){
	char *format = "% 10d";
	if(hex){
		format = " 0x%08x";
	}
	for(int i = 0; i < count; i++){
		if(*(array + i) > -1000000000 && !hex)
			printf(" ");
		printf(format, *(array + i));
		printf(" ");
	}
	printf(format, *(array + count));
	printf("\n");
}

void insertInOrder(int *array, int count, int insert){
	int i = 1;
	while(i < count && *(array + i + 1) < insert){
		*(array + i) = *(array + i + 1);
		i++;
	}
	*(array + i) = insert;
}
