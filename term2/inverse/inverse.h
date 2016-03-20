#ifndef inverse_h
#define inverse_h

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define A(i, j) a[index_i[i] * n + index_j[j]]
#define B(i, j) b[index_i[i] * n + index_j[j]]
#define eps (double) 1e-8

int gauss(double *a, double *b, int *index_i, int *index_j, int n);

#endif
