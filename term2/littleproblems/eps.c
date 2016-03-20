#include <stdio.h>

int main (void) {
	double eps = 1;
	while (1 + eps > 1) {
		eps /= 2;
	}
	printf("%le\n", eps);
	return 0;
}
