#include <stdio.h>
#include <ctype.h>

int main(){
	int input = 0, numSpaces = 0, numDigits = 0, numErr = 0, exit = 0;
	unsigned char *conv = (unsigned char*)&input;
	char err;
	int rc = scanf("%d", &input);
	while(rc >= 0){
		if(rc == 0){
			scanf("%c", &err);
			printf("OOPS, the character 0x%02x='%c' is not part of aan integer!\n", err, err);
			numErr++;
			exit = 1;
		} else {
			printf("val=%d=0x%08x: as characters:", input, input);
			for(int i = 0; i < 4; i++){
				
				printf(" 0x%02x=", conv[i]);
				
				if(conv[i] == '\0')
					printf("<null>");
				else if(!isprint(conv[i]))
					printf("?");
				else
					printf("'%c'", conv[i]);
				
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
