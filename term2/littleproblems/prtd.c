#include <stdio.h>
#include <inttypes.h>

typedef union {
	double d;
	uint64_t i;
} double_t;

void prtDouble (double x);
void prtmax (void);
void next(double x);

void prtDouble (double x) {
	double_t z;
	int i;
	z.d = x;
	for (i = 63; i >= 0; i--) {
		printf("%d", (z.i & UINT64_C(1) << i) ? 1 : 0);
		if (i == 63) printf(" ");
		else if (i == 52) printf(" ");
	}
	printf("\n");
}

void prtmax (void) {
	double_t z;
	int i;
	z.i = z.i & ~(UINT64_C(1) << 63);
	for (i = 62; i >= 0; i--) {
		z.i = z.i | (UINT64_C(1) << i);
	}
	z.i = z.i & ~(UINT64_C(1) << 52);
	prtDouble(z.d);
	printf("%le\n", z.d);
}

void next (double x) {
	double_t z;
	z.d = x;
	z.i += 1;
	prtDouble(z.d);
	printf("%.16lf\n", z.d);
	printf("%"PRIu64"\n", z.i);
}

int main (void) {
	double x;
	int i;
	x = 1;
	for (i = 1; i <= 56; i++) {
		x *= 2;
	}
	next(x);
	return 0;
}
