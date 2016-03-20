#ifndef chord_h
#define chord_h

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <limits.h>
#include <errno.h>

typedef double (*func_t)(double);
int chord(func_t f, double a, double b, double e, double *res);
double f1(double x);
double f2(double x);
int toInt(const char *s, int *x);
int toDouble(const char *s, double *x);
#endif
