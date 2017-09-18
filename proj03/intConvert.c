/* intConvert.c
 * Programmer: Ryan Sims
 * Date: 9/15/2017
 *
 * intConvert is a program that converts ints to 4-char arrays using pointer casting.
 * It accepts input of ints or chars from stdin until an EOF is read.
 * When an int is read, the 4 chars it contains are printed to stdout, and the  
 * number of spaces and digits it contains are counted.
 * If an int could not be read, the offending character is read, an error message
 * is printed to stdout, an error counter is incremented, and the return value
 * of main() is set to 1.
 * After an EOF is read, the number of digits, spaces, and errors is printed
 * to the screen, and the program returns from main.
 */

#include <stdio.h>
#include <ctype.h>

int main(){
	int input = 0, numSpaces = 0, numDigits = 0, numErr = 0, exit = 0;
	//conv is the int input as an unsigned char array
	unsigned char *conv = (unsigned char*)&input;
	char err;
	int rc = scanf("%d", &input);
	while(rc >= 0){
		//If int can't be read, read char
		if(rc == 0){
			scanf("%c", &err);
			printf("OOPS, the character 0x%02x='%c' is not part of an integer!\n", err, err);
			numErr++;
			exit = 1;
		} else {
			//Int read, convert to char array.
			printf("val=%d=0x%08x: as characters:", input, input);
			for(int i = 0; i < 4; i++){
				
				printf(" 0x%02x=", conv[i]);
				
				//Special cases: \0 -> <null>, non-printable -> ?, otherwise print char as is.
				if(conv[i] == '\0')
					printf("<null>");
				else if(!isprint(conv[i]))
					printf("?");
				else
					printf("'%c'", conv[i]);
				
				//Count digits and spaces
				if(conv[i] == ' ')
					numSpaces++;
				else if(isdigit(conv[i]))
					numDigits++;
			}
			printf("\n");
		}
		rc = scanf("%d", &input);
	}
	printf("\nTotals: spaces=%d digits=%d bad=%d\n", numSpaces, numDigits, numErr);
	return exit;
}
