/* squares.c
 * Programmer: Ryan Sims
 * Date: 9/9/17
 * 
 * Squares is a short program that prints squares of letters, slashes, and stars
 * to stdout. Squares takes one int size followed by any number of chars as input, and
 * will output squares of each char of dimension size*size.
 */


#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

//drawSquare draws a single square to stdout using size and fill.
int drawSquare(int size, char fill);

//Main function to control input and exit conditions
int main(void){
	//Variables to hold the inputs from stdin
	int size;
	char fill;
	
	//Flag to control output of newline.
	//Prevents extra newline from being printed at beginning of output
	int result = -1;
	
	//Check that size is found
	if(!scanf("%d", &size)){
		fprintf(stderr, "Size not found in input.\n");
		return 1;
	} else if(size <= 0){ //Check that size is in correct range
		fprintf(stderr, "Size must be positive and nonzero.\n");
		return 1;
	}
	//Scan characters until no more characters can be scanned
	//and use drawSquare to draw a square with each scanned char
	while(scanf("%c", &fill) > 0){
		if(result != -1)
			printf("\n");
		result = drawSquare(size, fill);
	}
	return 0;
}

/*
 * drawSquare draws a single square to stdout using size and fill.
 * If fill is not printable, drawSquare prints an error message and fails to draw a square.
 * Returns 1 on failure, 0 on success
 */
int drawSquare(int size, char fill){
	int i, j;
	//Check that fill is printable and print error and hex code if not
	if(!isprint(fill)){
		printf("The input character is not a printable character.  Hex=0x%02x\n", fill);
		return 1;
	}
	//Draw square of size * size
	for(i = 0; i < size; i++){
		for(j = 0; j < size; j++){
			//Check if current location is center of odd size
			if(i == (size - 1) / 2 && i == j && size % 2 != 0)
				printf("*");
			//Check if loc is on TL to BR diagonal
			else if(i == j)
				printf("\\");
			//Check if loc is on TR to BL diagonal
			else if(size - 1 - i == j)
				printf("/");
			//Check if loc is on top or bottom edge
			else if(i == 0 || i == size - 1)
				printf("-");
			//Check if loc is on left or right edge
			else if(j == 0 || j == size - 1)
				printf("|");
			//After all special cases checked, print fill
			else
				printf("%c", fill);
		}
		printf("\n");
	}
	return 0;
}
