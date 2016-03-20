#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAXSTR 100

int cmp(const void *irst, const void *econd) {
    const char ** first = (const char **) irst;
    const char ** second = (const char **) econd;
    int first0=0, second0=0, first1=0, second1=0;
    int j;
    
    for (j = 7; j >= 0; j--) {
        first0 += (1&((*first)[0]>>j)) * (1<<j);
    }
    for (j = 7; j >= 0; j--) {
        second0 += (1&((*second)[0]>>j)) * (1<<j);
    }
    for (j = 7; j >= 0; j--) {
        first1 += (1&((*first)[1]>>j)) * (1<<j);
    }
    for (j = 7; j >= 0; j--) {
        second1 += (1&((*second)[1]>>j)) * (1<<j);
    }
    
    
    if (first0 > second0) return 1;
    if (first0 < second0) return -1;
    if (first1 > second1) return 1;
    if (first1 < second1) return -1;
    return 0;
}

int main(int argc, char * argv[]) {
    char* *string;
    int now = 0;
    char symb;
    int i;
    int pos;
    int stringlen;
    
    if (argc != 2) {
        fprintf(stderr, "Command line arguments error\n");
        return -1;
    }
    
    string = malloc(MAXSTR * sizeof(char *));
    for (i = 0; i < MAXSTR; i++) {
        string[i] = malloc(2 * sizeof(char));
    }
    
    for (pos = 0, i = 0; ; pos++) {
        symb = argv[1][pos];
        if (symb == '\0') break;
    
        if ((1&(symb>>7)) == 1 && (1&(symb>>6)) == 1 && (1&(symb>>5)) == 0) {
            if (now == 1) {
                fprintf(stderr, "Not UTF, error 1\n");
                return -1;
            }
            now = 1;
            string[i][0] = symb;
        } else if ((1&(symb>>7)) == 1 && (1&(symb>>6)) == 0) {
            if (now == 0) {
                fprintf(stderr, "Not UTF, error 2\n");
                return -1;
            }
            now = 0;
            string[i][1] = symb;
            i++;
        } else if ((1&(symb>>7)) == 0) {
            if (now == 1) {
                fprintf(stderr, "Not UTF, error 3\n");
                return -1;
            }
            string[i][0] = 0;
            string[i][1] = symb;
            i++;
        } else {
            fprintf(stderr, "Not UTF, error 4\n");
            return -1;
        }
    }
    
    stringlen = i;
    
    qsort(string, stringlen, sizeof(char *), cmp);
    
    
    for (i = 0; i < stringlen; i++) {
        if (string[i][0] == '\0') {
            printf("%c", string[i][1]);
        } else {
            printf("%s", string[i]);
        }
    }
    
    return 0;
}