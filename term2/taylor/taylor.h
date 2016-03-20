#ifndef _TAYLOR_H_
#define _TAYLOR_H_
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <errno.h>
#include <math.h>
#include <string.h>

int toDouble(const char *s, double *x);
int toInt(const char *s, int *x);
int taylor(double x, double eps, double *res);

#endif
