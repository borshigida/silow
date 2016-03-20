#include "chord.h"

// Pi = 3.1415926535897932385
// 2 Pi = 6.2831853071795864769
// Sqrt Pi = 1.7724538509055160273
// Sqrt 2Pi = 2.5066282746310005024
// Sqrt 3/2Pi = 2.1708037636748029781

int toDouble (const char *s, double *x) {
    char *e;
    errno = 0;
    *x = strtod(s, &e);
    if (!errno && *e == '\0') return 0;
    return -1;
}

int toInt (const char *s, int *x) {
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
    const char *txt = "Usage ./chord <num> <a> <b> <eps>\n"
    "Test functions\n"
    "f1(x) = 2 * x * cos(x * x)\n"
    "f2(x) = e^(0.1x)sin(x)\n";
    int num;
    double a, b, e;
    double r;
    if (argc == 1) {
        printf("%s", txt);
    } else if (argc == 5) {
        if (toInt(argv[1], &num) == -1) {
            fprintf(stderr, "Error\n");
            return -1;
        }
        
        if (toDouble(argv[2], &a) == -1) {
            fprintf(stderr, "Error\n");
            return -1;
        }
        if (toDouble(argv[3], &b) == -1) {
            fprintf(stderr, "Error\n");
            return -1;
        }
        if (toDouble(argv[4], &e) == -1) {
            fprintf(stderr, "Error\n");
            return -1;
        }
        switch (num) {
            case 1:
                if (chord(f1, a, b, e, &r) == -1) {
                    fprintf(stderr, "Error\n");
                    return -1;
                }
                break;
            case 2: 
                if (chord(f2, a, b, e, &r) == -1) {
                    fprintf(stderr, "Error\n");
                    return -1;
                }
                break;
            default: 
                fprintf(stderr, "Error\n");
                return -1;
        }
        printf("%.16lf\n", r);	
    } else {
        fprintf(stderr, "Error\n");
        return -1;
    }
    
    return 0;
}
