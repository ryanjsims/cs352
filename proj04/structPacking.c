#include "proj04.h"
#include <stdio.h>
#include <stdlib.h>

int fillOneFoo(Foo *fooPtr);
void printInts(int *fooInts, int size);
void printOneBar(Bar *fuBar);
void printOneBaz(Baz *fooBaz);

int main(){
	Foo *array = malloc(4 * sizeof(Foo));
	Bar *fuBar = (Bar*)array;
	Baz *fooBaz = (Baz*)array;
	unsigned char *arrayAsChars = (unsigned char*)array;
	int *fooInt = (int*)array;
	int fill;
	int rc = scanf("%d", &fill);
	if(!(rc > 0)){
		fprintf(stderr, "Could not read int to fill the array.\n");
		return 1;
	} else if(fill > 255 || fill < 0){
		fprintf(stderr, "The input was out of the [0, 255] range.\n");
		return 1;
	}
	for(int i = 0; i < 4 * sizeof(Foo); i++){
		*(arrayAsChars + i) = fill;
	}
	for(int i = 0; i < 4; i++){
		if(fillOneFoo(array + i) != 0)
			return 1;
	}
	printf("sizeof(Foo)=%ld\n", sizeof(Foo));
	printf("sizeof(Bar)=%ld\n", sizeof(Bar));
	printf("sizeof(Baz)=%ld\n\n", sizeof(Baz));
	printInts(fooInt, (int)sizeof(Foo));
	for(int i = 0; i < 2; i++)
		printOneBar(fuBar + i);
	printf("\n");
	for(int i = 0; i < 2; i++)
		printOneBaz(fooBaz + i);
	return 0;
}

int fillOneFoo(Foo *fooPtr){
	int rc = scanf("%d %d %d %c %c", &(fooPtr->arr[0]), 
									 &(fooPtr->arr[1]),
									 &(fooPtr->arr[2]),
									 &(fooPtr->c1),
									 &(fooPtr->c2));
	if(rc < 5){
		fprintf(stderr, "Foo not completely filled!\n");
		return 1;
	}
	return 0;
}

void printInts(int* fooInts, int size){
	for(int i = 0; i < size; i++){
		printf("0x%08x", *(fooInts + i));
		if(i != size - 1)
			printf(" ");
	}
	printf("\n\n");
}

void printOneBar(Bar *fuBar){
	printf("Bar:  ");
	printf("x: 0x%08x=%d  ", fuBar->x, fuBar->x);
	printf("y: 0x%08x=%d  ", fuBar->y, fuBar->y);
	printf("z: 0x%08x=%d  ", fuBar->z, fuBar->z);
	printf("c1: 0x%02x  ", fuBar->c1);
	printf("c2: 0x%02x\n", fuBar->c2);
}

void printOneBaz(Baz *fooBaz){
	printf("Baz:  ");
	printf("x: 0x%08x=%d  ", fooBaz->x, fooBaz->x);
	printf("c1: 0x%02x  ", fooBaz->c1);
	printf("y: 0x%08x=%d  ", fooBaz->y, fooBaz->y);
	printf("c2: 0x%02x  ", fooBaz->c2);
	printf("z: 0x%08x=%d\n", fooBaz->z, fooBaz->z);
}
