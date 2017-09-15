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


	//TODO: put your code here
	int bugSwitch;
	char *formatString;
	if(scanf("%d", &bugSwitch) <= 0) {
		fprintf(stderr, "Could not read int as first input!\n");
		return 1;
	}
	if(bugSwitch != 0 && bugSwitch != 1) {
		fprintf(stderr, "Int was not 0 or 1!\n");
		return 1;
	}
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
	for(int i = 4; i >= -64; i--)
		printf("%c", buf1[i]);
	printf("\n");
	while(scanf(formatString, buf1) > 0) {
		int len = strlen(buf1) - 1;
		for(int i = len; i >= 0; i--)
			printf("%c", buf1[i]);
		printf("\n");
		printf("\tDEBUG: buf1='%s' buf2='%s' buf3='%s'\n", buf1, buf2, buf3);
		if(len > 31)
			printf("\tOOPS: The word to reverse was too long!  len=%d\n", len + 1);
	}
	return 0;
}

