#include <stdio.h>
#include <stdlib.h>
#include "integrate.h"

#define MAX_ATT 25

double integrateN(func_t f, double a, double b, int n) {
	double h = (b - a) / n;
	int i; 
	double *x = (double*) malloc((n + 1) * sizeof(double));
	double *fx = (double*) malloc((n + 1) * sizeof(double));
	double first_sum = 0, second_sum = 0;

	for (i = 0; i <= n; i++) {
		x[i] = a + i * h;
		fx[i] = f(x[i]);
	}
	
	for (i = 2; i <= n - 2; i += 2) {
		first_sum += fx[i];
	}
	for (i = 1; i <= n - 1; i += 2) {
		second_sum += fx[i];
	}
	return h / 3 * (fx[0] + fx[n] + 2 * first_sum + 4 * second_sum);
}

int integrate(func_t f, double a, double b, double e, double *r) {
	int n = 2;
	int i;
	double delta;
	double I_2n, I_n, I;
	if (a >= b) return -1;
	if (e <= 0) return -1;
	I_n = integrateN(f, a, b, n);
	I_2n = integrateN(f, a, b, 2 * n);
	n = 2 * n;
	for (i = 0; i < MAX_ATT; i++) {
		I_n = I_2n;
		I_2n = integrateN(f, a, b, 2 * n);
		delta = (I_2n - I_n) / 15;
		I = I_2n + delta;	
		if (-e < delta && delta < e)
			break;
		else 
			n = 2 * n;
	}
	if (-e < delta && delta < e) {
		*r = I;
		return 0;
	} else 
		return -1;
}
