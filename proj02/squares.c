#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

int drawSquare(int size, char fill);

int main(void){
	int *size = (int*)malloc(sizeof(int));
	char *fill = (char*)malloc(sizeof(char));
	int result = -1;
	if(!scanf("%d", size)){
		fprintf(stderr, "Size not found in input.\n");
		return 1;
	} else if(*size <= 0){
		fprintf(stderr, "Size must be positive and nonzero.\n");
		return 1;
	}
	while(scanf("%c", fill) > 0){
		if(result != -1)
			printf("\n");
		result = drawSquare(*size, *fill);
	}
	free(size);
	free(fill);
	return 0;
}

int drawSquare(int size, char fill){
	int i, j;
	if(!isprint(fill)){
		printf("The input character is not a printable character.  Hex=0x%02x\n", fill);
		return 1;
	}
	for(i = 0; i < size; i++){
		for(j = 0; j < size; j++){
			if(i == (size - 1) / 2 && i == j && size % 2 != 0)
				printf("*");
			else if(i == j)
				printf("\\");
			else if(size - 1 - i == j)
				printf("/");
			else if(i == 0 || i == size - 1)
				printf("-");
			else if(j == 0 || j == size - 1)
				printf("|");
			else
				printf("%c", fill);
		}
		printf("\n");
	}
	return 0;
}
