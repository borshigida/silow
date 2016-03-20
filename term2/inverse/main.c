#include "inverse.h"

int gauss(double *a, double *b, int *index_i, int *index_j, int n) {
    int i, j, i1, j1;
    // int i0, j0;
    double mx;
    int mx_i;
    int inttemp;
    double nowel;
    
    // прямой ход
    for (j = 0; j < n; j++) {
        mx = 0;
        mx_i = -1;
        for (i = j; i < n; i++) {
            if (fabs(A(i, j)) > mx) {
                mx = fabs(A(i, j));
                mx_i = i;
            }
        }
        if (mx_i == -1 || fabs(mx) < eps) return -1;
        inttemp = index_i[j];
        index_i[j] = index_i[mx_i];
        index_i[mx_i] = inttemp;
        
//        for (i0 = 0; i0 < n; i0++) {
//            for (j0 = 0; j0 < n; j0++) {
//                printf("%lf ", A(i0, j0));
//            }
//            printf("\n");
//        }
//        printf("\n");
        
        nowel = A(j, j);
        for (j1 = 0; j1 < n; j1++) {
            A(j, j1) = A(j, j1) / nowel;
            B(j, j1) = B(j, j1) / nowel;
            //printf("j = %d, j1 = %d\n", j, j1);
        }
        
//        for (i0 = 0; i0 < n; i0++) {
//            for (j0 = 0; j0 < n; j0++) {
//                printf("%lf ", A(i0, j0));
//            }
//            printf("\n");
//        }
//        printf("\n");
        
        for (i1 = j + 1; i1 < n; i1++) {
            nowel = A(i1, j);
            for (j1 = 0; j1 < n; j1++) {
                A(i1, j1) -= A(j, j1) * nowel;
                B(i1, j1) -= B(j, j1) * nowel;
            }
        }
        
//        for (i0 = 0; i0 < n; i0++) {
//            for (j0 = 0; j0 < n; j0++) {
//                printf("%lf ", A(i0, j0));
//            }
//            printf("\n");
//        }
//        printf("\n");
    }
    
    // обратный ход
    for (j = n - 1; j > 0; j--) {
        for (i = 0; i < j; i++) {
            nowel = A(i, j);
            for (j1 = 0; j1 < n; j1++) {
                A(i, j1) -= A(j, j1) * nowel;
                B(i, j1) -= B(j, j1) * nowel;
            }
        }
    }
    
    return 0;
}

int main(void) {
    double *a, *b;
    int *index_i, *index_j;
    int n;
    int i, j;
    FILE *fin = fopen("input.txt", "r");
    FILE *fout = fopen("output.txt", "w");
    
    if (fscanf(fin, "%d", &n) != 1 || n <= 0) return -1;
    a = malloc(sizeof(double) * n * n);
    b = malloc(sizeof(double) * n * n);
    index_i = malloc(sizeof(int) * n);
    index_j = malloc(sizeof(int) * n);
    for (i = 0; i < n; i++) index_i[i] = i;
    for (j = 0; j < n; j++) index_j[j] = j;
    
    for (i = 0; i < n; i++) {
        for (j = 0; j < n; j++) {
            if (fscanf(fin, "%lf", &A(i, j)) != 1) return -1;
            if (i == j) B(i, j) = 1;
            else B(i, j) = 0;
        }
    }
    
    if (gauss(a, b, index_i, index_j, n) == -1) return -1;
    
//    for (i = 0; i < n; i++) {
//        for (j = 0; j < n; j++) {
//            printf("%lf ", A(i, j));
//        }
//        printf("\n");
//    }
//    printf("\n");
    
    for (i = 0; i < n; i++) {
        for (j = 0; j < n; j++) {
            fprintf(fout, "%lf ", B(i, j));
        }
        fprintf(fout, "\n");
    }
    fprintf(fout, "\n");
    return 0;
}
