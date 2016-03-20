#include "taylor.h"

int toDouble(const char *s, double *x) {
    char *e;
    errno = 0;
    *x = strtod(s, &e);
    if (!errno && *e == '\0') return 0;
    return -1;
}

int toInt(const char *s, int *x) {
    long l;
    char *e;
    errno = 0;
    l = strtol(s, &e, 10);
    if (!errno && *e == '\0' && INT_MIN <= l && l <= INT_MAX) {
        *x = (int) l;
        return 0;
    } else
        return -1;
}

int main(int argc, char* argv[]) {
    double x, eps;
    double res;
    if (argc == 1) {
        printf("Usage: ./%s <x> <eps>\n", argv[0]);
	return 0;
    } else if (argc == 3) {
        if (toDouble(argv[1], &x) == -1) {
            fprintf(stderr, "Error\n");
            return -1;
        }
        if (toDouble(argv[2], &eps) == -1) {
            fprintf(stderr, "Error\n");
            return -1;
        }
        
        if (taylor(x, eps, &res) == -1) {
            fprintf(stderr, "Error\n");
            return -1;
        }
    }
    
    printf("%.16lf\n", res);
    printf("dist = %.16lf\n", res - log(x));
    //printf("e = %.16lf, sqrt e = %.16lf", M_E, sqrt(M_E));
    
    return 0;
}