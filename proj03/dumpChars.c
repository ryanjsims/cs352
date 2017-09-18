/* dumpChars.c
 * Programmer: Ryan Sims
 * Date: 9/15/2017
 * 
 * dumpChars is a program that prints out the hex values of characters from stdin.
 * It accepts input on stdin until an EOF is read by scanf.
 * The output is lines of 16 bytes in hex format separated by spaces,
 * grouped into four 4 byte groups with additional spaces between them.
 */

#include <stdio.h>

int main(){
	char curr;
	int rc = scanf("%c", &curr);
	//For loop is used for convenient counter variable.
	//Loop condition only depends on scanf's return code rc.
	for(int i = 1; rc > 0; i++){
		printf("%02x", curr);
		//Check if there is another char to print after current
		rc = scanf("%c", &curr);
		//At the end of the line or string there should be no spaces
		//So if no new char present or 16 chars have been printed, print \n
		if(rc <= 0 || i % 16 == 0)
			printf("\n");
		//More chars to print, so print space
		else{
			printf(" ");
			//Every 4 chars should be separated with additional space
			if(i % 4 == 0)
				printf(" ");
		}
	}
	return 0;
}
