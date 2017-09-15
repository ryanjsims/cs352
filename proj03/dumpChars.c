#include <stdio.h>

int main(){
	char curr;
	int rc = scanf("%c", &curr);
	for(int i = 1; rc > 0; i++){
		printf("%02x", curr);
		rc = scanf("%c", &curr);
		if(rc <= 0 || i % 16 == 0)
			printf("\n");
		else{
			printf(" ");
			if(i % 4 == 0)
				printf(" ");
		}
	}
	return 0;
}
