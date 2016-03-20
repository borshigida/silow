#define _XOPEN_SOURCE 500
#include <ftw.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INF 1000

char *oext;

int fn(const char *fpath, const struct stat *sb, int typeflag, struct FTW *ftwbuf);

int fn(const char *fpath, const struct stat *sb, int typeflag, struct FTW *ftwbuf) {
    char *ext;
    char *q;
    
    ext = malloc(sizeof(char) * INF);
    //ext = (char*) "";
    sb = sb;
    typeflag = typeflag;
    ftwbuf = ftwbuf;
    
    if (typeflag == FTW_F) {
	q = (char*) (fpath + ftwbuf->base);
	//printf("%s\n", q);
	while (*q != '.' && *q != '\0') q++;
	if (*q == '.') {
	    sscanf(q, ".%s", ext);
	}
	if (strcmp(oext, ext) == 0) printf("%s\n", fpath);
    }

    return 0;
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
	fprintf(stderr, "Error\n");
	return -1;
    }
    
    oext = argv[2];
    
    nftw(argv[1], fn, 0, 0);
    
    printf("\n");
    
    return 0;
}