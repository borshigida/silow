#ifndef _INTEGRATE_H_
#define _INTEGRATE_H_
typedef double (*func_t)(double);
double integrateN(func_t f, double a, double b, int n);
int integrate(func_t f, double a, double b, double e, double *r); 
double f1(double x);
double f2(double x);
double f3(double x);
double f4(double x);
double f5(double x);
double f6(double x);
double f7(double x);
int toInt(const char *s, int *x);
int toDouble(const char *s, double *x);
#endif 
