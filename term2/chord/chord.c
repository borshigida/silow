#include "chord.h"

#define MAX_ATT 100
#define EPS (int) 1e-9

int chord(func_t f, double a, double b, double e, double *res) {
    double x=a, x0=b;
    int i;
    if (f(x0) * f(x) >= 0) {
        return -1;
    }
    
    for (i=0; i < MAX_ATT; i++) {
        if (-EPS < f(x) - f(x0) && f(x) - f(x0) < EPS) return -1;
        x = x - (x - x0) / (f(x) - f(x0)) * f(x);
        printf("%lf\n", x);
        if (x < a || x > b) {
            return -1;
        }
        if (f(x - e) * f(x + e) < 0) break;
    }
    
    if (!(f(x - e) * f(x + e) < 0)) {
        return -1;
    }
    *res = x;
    return 1;
}
