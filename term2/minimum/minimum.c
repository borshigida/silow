#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "minimum.h"

#define MAX_ATT 100

int minimum(func_t f, double a, double b, double e, double *res) {
	double r = (3 - sqrt(5)) / 2;
	double c = a + r * (b - a);
	double d = b - r * (b - a);
	int i;
	for (i = 0; i < MAX_ATT; i++) {
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
