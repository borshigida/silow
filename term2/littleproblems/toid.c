#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <limits.h>

int toDouble (const char *s, double *x);
int toInt (const char *s, int *x);

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

int main (int argc, char* argv[]) {
	double x = 0;
	int y;
	int i;
	for (i = 1; i < argc; i++) {
		printf("%s ", argv[i]);
	}
	printf("\n");
	
	for (i = 1; i < argc; i++) {
		if (toDouble(argv[i], &x) != 0) printf("None ");
		else printf("%lf ", x);
	}
	printf("\n");

	for (i = 1; i < argc; i++) {
		if (toInt(argv[i], &y) != 0) printf("None ");
		else printf("%d ", y);
	}
	printf("\n");
	return 0;
}
