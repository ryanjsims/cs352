#include <stdio.h>

int main(){
	int x, a, b, c;
	int *y = &x;
	a = 4;
	b = a * a;
	c = a << 4;
	*y = 2;
	for(int i = -3; i < 4; i++){
		printf("%d\n", y[i]);
	}
	return 0;
}
