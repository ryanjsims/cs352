#include <stdio.h>

int main(void){
	int num, cnt, mod, product = 1, total = 0;
	if(scanf("%d %d %d", &num, &cnt, &mod) < 3){
		fprintf(stderr, "Did not read all 3 parameters.\n");
		return 1;
	} else if(num < 2 || mod < 2){
		fprintf(stderr, "num and modulo must both be >= 2");
		return 1;
	}
	while(cnt > 0){
		printf("%d", product);
		if(product % mod == 0){
			printf(" is a multiple of %d", mod);
			total++;
		}
		printf("\n");
		product *= num;
		cnt--;
	}
	printf("TOTAL: %d multiples of %d\n", total, mod);
	return 0;
}
