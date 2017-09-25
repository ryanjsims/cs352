/* structPacking.c
 * Programmer: Ryan Sims
 * Date: 9/25/2017
 *
 * structPacking casts between different struct pointers to see
 * what happens to the memory.
 * Returns 1 if any input errors are detected, else returns 0
 */

#include "proj04.h"
#include <stdio.h>
#include <stdlib.h>

int fillOneFoo(Foo *fooPtr);
void printInts(int *fooInts, int size);
void printOneBar(Bar *fuBar);
void printOneBaz(Baz *fooBaz);

int main(){
	//Allocate array of Foo structs and create various pointer casts.
	Foo *array = malloc(4 * sizeof(Foo));
	if (array == NULL) {
		fprintf(stderr, "Could not allocate memory for array.\n");
		return 1;
	}
	Bar *fuBar = (Bar*)array;
	Baz *fooBaz = (Baz*)array;
	unsigned char *arrayAsChars = (unsigned char*)array;
	int *fooInt = (int*)array;

	//Get fill byte and error check
	int fill;
	int rc = scanf("%d", &fill);
	if(!(rc > 0)){
		fprintf(stderr, "Could not read int to fill the array.\n");
		return 1;
	} else if(fill > 255 || fill < 0){
		fprintf(stderr, "The input was out of the [0, 255] range.\n");
		return 1;
	}

	//Fill array with fill byte
	for(int i = 0; i < 4 * sizeof(Foo); i++){
		*(arrayAsChars + i) = (unsigned char)fill;
	}

	//Fill array with Foos, error checking input
	for(int i = 0; i < 4; i++){
		if(fillOneFoo(array + i) != 0)
			return 1;
	}

	//Print sizes, ints in array, bar array, and baz array
	printf("sizeof(Foo)=%ld\n", sizeof(Foo));
	printf("sizeof(Bar)=%ld\n", sizeof(Bar));
	printf("sizeof(Baz)=%ld\n\n", sizeof(Baz));
	printInts(fooInt, (int)sizeof(Foo));
	for(int i = 0; i < 2; i++)
		printOneBar(fuBar + i);
	printf("\n");
	for(int i = 0; i < 2; i++)
		printOneBaz(fooBaz + i);

	free(array);
	return 0;
}

/*
 * fillOneFoo
 * Takes a pointer to a Foo struct and fills it with input from stdin.
 * returns 0 on success, 1 on error.
 */
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

/*
 * printInts
 * Takes a pointer to an array of ints and the number of ints in the array as parameters,
 * then prints the ints in hex format.
 * No return value.
 */
void printInts(int* fooInts, int size){
	for(int i = 0; i < size; i++){
		printf("0x%08x", *(fooInts + i));
		if(i != size - 1)
			printf(" ");
	}
	printf("\n\n");
}

/*
 * printOneBar
 * Takes a pointer to a Bar struct as a parameter and prints the values contained by
 * the Bar. No return value.
 */
void printOneBar(Bar *fuBar){
	printf("Bar:  ");
	printf("x: 0x%08x=%d  ", fuBar->x, fuBar->x);
	printf("y: 0x%08x=%d  ", fuBar->y, fuBar->y);
	printf("z: 0x%08x=%d  ", fuBar->z, fuBar->z);
	printf("c1: 0x%02x  ", fuBar->c1);
	printf("c2: 0x%02x\n", fuBar->c2);
}

/*
 * printOneBaz
 * Takes a pointer to a Baz struct as a parameter and prints the values contained by
 * the Baz. No return value.
 */
void printOneBaz(Baz *fooBaz){
	printf("Baz:  ");
	printf("x: 0x%08x=%d  ", fooBaz->x, fooBaz->x);
	printf("c1: 0x%02x  ", fooBaz->c1);
	printf("y: 0x%08x=%d  ", fooBaz->y, fooBaz->y);
	printf("c2: 0x%02x  ", fooBaz->c2);
	printf("z: 0x%08x=%d\n", fooBaz->z, fooBaz->z);
}
