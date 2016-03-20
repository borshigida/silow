#include "taylor.h"

int taylor(double x, double eps, double *res) {
    int n, i;
    double y;
    double pow = 1;
    double a;
    if (x < 0) return -1;
    if (x < 1) {
        if (taylor(x, eps, res) == -1) {
            return -1;
        }
        *res = -*res;
        return 0;
    }
    
    
    for (n = 1; n < 1000; n++) {
        pow *= sqrt(M_E);
        if (x < 2 * pow) {
            y = x / pow - 1;
            break;
        }
    }
    if (n == 1000) return -1;
    
    a = y;
    *res = y;
    //printf("y = %lf\n", y);
    for (i = 2; i < 1000; i++) {
	a = (i - 1) * a * y / (double) i;
	if (i % 2) *res += a;
	else *res -= a;
	//printf("i = %d, *res = %lf\n", i, *res);
	if (-eps < a && a < eps) break;
    }
    
    *res += n / (double) 2;
    return 0;
}
