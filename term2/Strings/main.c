#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAXSTR 100

int main(int argc, char *argv[]) {
    FILE *fin, *fout;
    char str[MAXSTR];
    char word[MAXSTR];
    //char newword[MAXSTR];
    int i = 0;
    int j = 0;
    char* *defined = malloc(sizeof(char *) * MAXSTR);
    int defined_size = 0;
    int found = 0;
    int* stack = malloc(MAXSTR * sizeof(int));
    char* sstack = malloc(MAXSTR * sizeof(char));
    int top = 0;
    int ignoring = -1;
    int it_is_directive;
    int print;
    
    for (i = 0; i < MAXSTR; i++) {
        defined[i] = malloc(MAXSTR * sizeof(char));
    }
    
    if (argc != 2) {
        fprintf(stderr, "Error\n");
        return -1;
    }
    
    fin = fopen(argv[1], "r");
    if (fin == NULL) {
        fprintf(stderr, "Error opening input file\n");
        return -1;
    }
    fout = fopen("output.txt", "w");
    if (fout == NULL) {
        fprintf(stderr, "Error opening output file\n");
        return -1;
    }
    
    while (!feof(fin)) {
        fgets(str, MAXSTR, fin);
        print = 1;
        
        // === define ===
        it_is_directive = 0;
        i = 0;
        sscanf(str, "#define%n", &i);
        if (i != 0) {
            it_is_directive = 1;
        }
        j = 0;
        while(str[i] == ' ' && str[i] != '\0') {
            i++; j++;
        }
        if (j == 0 || str[i] == '\n' || str[i] == '\0') {
            it_is_directive = 0;
        }
    
        if (it_is_directive && ignoring == -1) {
            print = 0;
            j = 0;
            while (!isspace(str[i]) && str[i] != '\0') {
                word[j] = str[i];
                i++; j++;
            }
            word[j] = '\0';
            
            found = 0;
            for (int i = 0; i < MAXSTR; i++) {
                if (strcmp(defined[i], word) == 0) {
                    found = 1;
                    break;
                }
            }
            if (found == 0) {
                strcpy(defined[defined_size], word);
                defined_size++;
            }
            
//            printf("defined: ");
//            for (int i = 0; i < 3; i++) printf("%s ", defined[i]);
//            printf("\n");
        }
        // === end define ===
        
        // === ifdef ===
        it_is_directive = 0;
        i = 0;
        sscanf(str, "#ifdef%n", &i);
        if (i != 0) {
            it_is_directive = 1;
        }
        j = 0;
        while(str[i] == ' ' && str[i] != '\0') {
            i++; j++;
        }
        if (j == 0 || str[i] == '\0') {
            it_is_directive = 0;
        }
        
        if (it_is_directive) {
            print = 0;
            j = 0;
            while (!isspace(str[i]) && str[i] != '\0') {
                word[j] = str[i];
                i++; j++;
            }
            word[j] = '\0';
            
            found = 0;
            for (int i = 0; i < MAXSTR; i++) {
                if (strcmp(defined[i], word) == 0) {
                    found = 1;
                    break;
                }
            }
            if (found == 1) {
                stack[top] = 1;
                sstack[top] = 'i';
                top++;
            } else {
                stack[top] = 0;
                sstack[top] = 'i';
                top++;
                if (ignoring == -1) ignoring = top;
            }
        }
        // === end ifdef ===
        
        // === else ===
        it_is_directive = 0;
        i = 0;
        sscanf(str, "#else%n", &i);
        if (i != 0 && (isspace(str[i]) || str[i] == '\0')) {
            it_is_directive = 1;
        }
        if (it_is_directive) {
            print = 0;
            if (top == 0) {
                fprintf(stderr, "Error\n");
                return -1;
            }
            if (sstack[top - 1] == 'e') {
                fprintf(stderr, "Error\n");
                return -1;
            }
            sstack[top - 1] = 'e';
            if (stack[top - 1] == 1 && ignoring == -1) ignoring = top;
            else if (stack[top - 1] == 0 && ignoring == top) ignoring = -1;
        }
        // === end else ===
        
        // === endif ===
        it_is_directive = 0;
        i = 0;
        sscanf(str, "#endif%n", &i);
        if (i != 0 && (isspace(str[i]) || str[i] == '\0')) {
            it_is_directive = 1;
        }
        if (it_is_directive) {
            print = 0;
            if (top == 0 || sstack[top - 1] == 'i') {
                fprintf(stderr, "Error\n");
                return -1;
            }
            if (top == ignoring) ignoring = -1;
            if (top < ignoring) {
                fprintf(stderr, "Something's terribly wrong!\n");
            }
            top--;
        }
        // === end endif ===
        
        if (print && ignoring == -1) {
            fprintf(fout, "%s", str);
        }
    }
    
//    printf("defined: ");
//    for (int i = 0; i < 15; i++) printf("%s; ", defined[i]);
//    printf("\n");
//    
//    printf("stack: ");
//    for (int i = 0; i < 15; i++) printf("%d; ", stack[i]);
//    printf("\n");
//    
//    printf("sstack: ");
//    for (int i = 0; i < 15; i++) printf("%c; ", sstack[i]);
//    printf("\n");
    
    if (top != 0) {
        fprintf(stderr, "Error\n");
        return -1;
    }
    
    fclose(fin); fclose(fout);
    return 0;
}