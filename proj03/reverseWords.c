/* reverseWords.c
 *
 * Author: Ryan Sims
 *         (skeleton by Russ Lewis)
 *
 * Reverses the words of output.  Has some (intentional) buffer-overflow
 * bugs.
 */

#include <stdio.h>
#include <string.h>

int main()
{
	char buf1[32];
	char buf2[32];
	char buf3[32];
	

	/* initialize the strings in all three buffers */
	strcpy(buf1, "BUF1");
	strcpy(buf2, "BUF2");
	strcpy(buf3, "BUF3");


	int bugSwitch;
	char *formatString;

	//Check error cases related to bug switch not being read or out of range
	if(scanf("%d", &bugSwitch) <= 0) {
		fprintf(stderr, "Could not read int as first input!\n");
		return 1;
	}
	if(bugSwitch != 0 && bugSwitch != 1) {
		fprintf(stderr, "Int was not 0 or 1!\n");
		return 1;
	}


	int returnValue = 0;
	//If bug switch is 1, put no limit on the format string.
	switch(bugSwitch){
		case 0:
			formatString = "%31s";
			break;
		case 1:
			formatString = "%s";
			break;
		default:
			return 1; //Should never happen
	}
	//Read input into buf1 until EOF passed
	while(scanf(formatString, buf1) > 0) {
		int len = strlen(buf1) - 1;
		//Print chars backwards
		for(int i = len; i >= 0; i--)
			printf("%c", buf1[i]);
		printf("\n");
		printf("\tDEBUG: buf1='%s' buf2='%s' buf3='%s'\n", buf1, buf2, buf3);
		//If overflow detected, print message and set return value non-zero
		if(len > 31){
			printf("\tOOPS: The word to reverse was too long!  len=%d\n", len + 1);
			returnValue = 1;
		}
	}
	return returnValue;
}

