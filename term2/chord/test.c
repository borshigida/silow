#include "chord.h"

double f1(double x) {
    //return sin(x * x) + 0.4;
    return 2 * x * cos(x * x);
}

double f2(double x) {
    return exp(0.1 * x) * cos(x) + 0.1 * exp(0.1 * x) * sin(x);
}
