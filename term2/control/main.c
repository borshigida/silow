#include "control.h"

double a = 0.0, b = 12.0, eint, emin;

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
    double x;
    double first, second;
    double integral;
    double dist;
    a = 0; b = 12;
    if (argc == 1) {
        printf("Usage: ./%s <eint> <emin>\n", argv[0]);
	return 0;
    } else if (argc == 3) {
        if (toDouble(argv[1], &eint) == -1) {
            fprintf(stderr, "Error\n");
            return -1;
        }
        if (toDouble(argv[2], &emin) == -1) {
            fprintf(stderr, "Error\n");
            return -1;
        }
        
        if (minimum(g, a, b, emin, &x) == -1) {
            fprintf(stderr, "Error\n");
            return -1;
        }
        if (integrate(f, a, x, eint, &first) == -1) {
            fprintf(stderr, "Error\n");
            return -1;
        }
        if (integrate(f, x, b, eint, &second) == -1) {
            fprintf(stderr, "Error\n");
            return -1;
        }
        if (integrate(f, a, b, eint, &integral) == -1) {
            fprintf(stderr, "Error\n");
            return -1;
        }
        dist = fabs(first - second);
    }
    
    printf("x = %.16lf\n", x);
    printf("first = %.16lf\n", first);
    printf("second = %.16lf\n", second);
    printf("integral = %.16lf\n", integral);
    printf("dist = %.16lf\n", dist);
    
    return 0;
}