#include <stdio.h>
#include <math.h>
#include "minimum.h"

double f1(double x) {
	return fabs(sin(x * x));
}

double f2(double x) {
	return fabs(exp(0.1 * x) * sin(x));
}
