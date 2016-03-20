#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <errno.h>
#include <math.h>
#include "integrate.h"

int toDouble (const char *s, double *x) {
	char *e;
	errno = 0;
	*x = strtod(s, &e);
	if (!errno && *e == '\0') return 0;
	return -1;
}

int toInt (const char *s, int *x) {
	long l;
	char *e;
	errno = 0;
	l = strtol(s, &e, 10);
	if (!errno && *e == '\0' && INT_MIN <= l && l <= INT_MAX) {
		*x = (int) l;
		return 0;
	} else 
		return -1;
}

int main(int argc, char* argv[]) {
	const char *txt = "Usage ./integrate <num> <a> <b> <eps>\n"
			  "Test functions\n"
			  "f1(x) = 2x\n"
			  "f2(x) = 3x^2\n"
			  "f3(x) = 4x^3\n"
			  "f4(x) = 5x^4\n"
			  "f5(x) = 1 / (1 + x^2)\n"
			  "f6(x) = sqrt(1 - x^2)\n"
			  "f7(x) = 2 exp(2x)\n";
	int num;
	double a, b, e;
	double r;
	if (argc == 1) {
		printf("%s", txt);
	} else {
		if (toInt(argv[1], &num) == -1) {
			fprintf(stderr, "Error\n");
			return -1;
		}

		if (toDouble(argv[2], &a) == -1) {
			fprintf(stderr, "Error\n");
			return -1;
		}
		if (toDouble(argv[3], &b) == -1) {
			fprintf(stderr, "Error\n");
			return -1;
		}	
		if (toDouble(argv[4], &e) == -1) {
			fprintf(stderr, "Error\n");
			return -1;
		}
		switch (num) {
			case 1: 
				if (integrate(f1, a, b, e, &r) == -1) {
					fprintf(stderr, "Error\n");
					return -1;
				}
				break;
			case 2: 
				if (integrate(f2, a, b, e, &r) == -1) {
					fprintf(stderr, "Error\n");
					return -1;
				}
				break;
			case 3: 
				if (integrate(f3, a, b, e, &r) == -1) {
					fprintf(stderr, "Error\n");
					return -1;
				}
				break;
			case 4: 
				if (integrate(f4, a, b, e, &r) == -1) {
					fprintf(stderr, "Error\n");
					return -1;
				}
				break;
			case 5: 
				if (integrate(f5, a, b, e, &r) == -1) {
					fprintf(stderr, "Error\n");
					return -1;
				}
				break;
			case 6: 
				if (integrate(f6, a, b, e, &r) == -1) {
					fprintf(stderr, "Error\n");
					return -1;
				}
				break;
			case 7: 
				if (integrate(f7, a, b, e, &r) == -1) {
					fprintf(stderr, "Error\n");
					return -1;
				}
				break;
			default: 
				fprintf(stderr, "Error\n");
				return -1;
		}
		printf("%.16lf\n", r);
		printf("Dist pi/4 pi/2 (e^2 - 1)\n");
		printf("%.16lf %.16lf %.16lf\n", r - M_PI / 4, r - M_PI / 2, r - M_E * M_E + 1);	
	}

	return 0;
}
