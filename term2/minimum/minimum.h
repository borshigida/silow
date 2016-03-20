#ifndef _MINIMUM_H_
#define _MINIMUM_H_
typedef double (*func_t)(double);
int minimum(func_t f, double a, double b, double e, double *res); 
double f1(double x);
double f2(double x);
int toInt(const char *s, int *x);
int toDouble(const char *s, double *x);
#endif 
