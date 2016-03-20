#ifndef _MYCONTROL_H_
#define _MYCONTROL_H_

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <errno.h>
#include <limits.h>
#define MAX_ATT 25
#define MAX_ATT2 100

typedef double (*func_t)(double);
double integrateN(func_t f, double a, double b, int n);
int integrate(func_t f, double a, double b, double e, double *r);
int minimum(func_t f, double a, double b, double e, double *res); 
double f(double x);
double g(double x);
int toInt(const char *s, int *x);
int toDouble(const char *s, double *x);

#endif