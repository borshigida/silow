#include "control.h"

extern double a, b, eint, emin;
 
 double integrateN(func_t f, double a, double b, int n) {
	double h = (b - a) / n;
	int i; 
	//double *x = (double*) malloc((n + 1) * sizeof(double));
	//double *fx = (double*) malloc((n + 1) * sizeof(double));
	double x, fx;
	double first_sum = 0, second_sum = 0;

	for (i = 0; i <= n; i++) {
		x = a + i * h;
		fx = f(x);
		if (i % 2 == 0 && i >= 2 && i <= n - 2) first_sum += fx;
		if (i % 2 == 1 && i >= 1 && i <= n - 1) second_sum += fx;
	}
	
	//for (i = 2; i <= n - 2; i += 2) {
	//	first_sum += fx[i];
	//}
	//for (i = 1; i <= n - 1; i += 2) {
	//	second_sum += fx[i];
	//}
	return h / 3 * (f(a) + f(b) + 2 * first_sum + 4 * second_sum);
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

int minimum(func_t f, double a, double b, double e, double *res) {
	double r = (3 - sqrt(5)) / 2;
	double c = a + r * (b - a);
	double d = b - r * (b - a);
	int i;
	for (i = 0; i < MAX_ATT2; i++) {
	  if (f(c) < f(d)) {
	      b = d;
	      d = c;
	      c = a + r * (b - a);
	  } else {
	      a = c;
	      c = d;
	      d = b - r * (b - a);
	  }
	  
	  if (-e < a - b && a - b < e) 
	    break;
	}
	
	if (-e < a - b && a - b < e) {
	    *res = (a + b) / 2;
	    return 0;
	} else 
	    return -1;
}

double f(double x) {
	return log(x + 1) + sqrt(x);
}

double g(double x) {
	double first, second; 
	integrate(f, a, x, eint, &first);
	integrate(f, x, b, eint, &second);
	return fabs(first - second);
}

