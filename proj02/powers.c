/* powers.c
 * Programmer: Ryan Sims
 * Date: 9/9/17
 *
 * Powers is a short program that iteratively prints out powers of a base number.
 * It accepts input from stdin in the form of 3 ints: number, count, and modulo.
 * It then prints out the powers of number from 0 to count - 1, and if any of them are
 * multiples of modulo, it notes that event. After all powers are output, it
 * prints the total number of multiples to the screen.
 */

#include <stdio.h>

int main(void){
	int num, cnt, mod, product = 1, total = 0;
	if(scanf("%d %d %d", &num, &cnt, &mod) < 3){ //Check that all parameters were found.
		fprintf(stderr, "Did not read all 3 parameters.\n");
		return 1;
	} else if(num < 2 || mod < 2){ //Ensure we get interesting results with num and mod > 1
		fprintf(stderr, "num and modulo must both be > 1");
		return 1;
	}
	//Print cnt - 1 powers of num by decrementing cnt to 0
	while(cnt > 0){
		printf("%d", product);
		//Check for multiples of mod using the modulo operator
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
