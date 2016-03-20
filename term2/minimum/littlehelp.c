#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <errno.h>
#include <math.h>
#include "minimum.h"

int main(void) {
	printf("sqrt pi %.16lf\n", sqrt(M_PI));
	printf("sqrt 2pi %.16lf\n", sqrt(2 * M_PI));
	printf("pi %.16lf\n", M_PI);
	printf("2pi %.16lf\n", 2 * M_PI);
	return 0;
}
