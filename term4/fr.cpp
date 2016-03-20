#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>

int main(int argc, char* argv[]) {
    int fd = open(argv[1], O_RDONLY);
    
    if (fd == -1) {
        printf("Error opening file\n");
        return -1;
    }

    if (argc > 2) {
        printf("Too many arguments\n");
        return -1;
    }
    
    char buf[1024];
    
    size_t len;
    ssize_t s;
    while(true) {
        s = read(fd, &len, sizeof len);
        if (s == 0)
            break;
        if (s != sizeof len) {
            printf("Error reading\n");
            return -1;
        }
        s = read(fd, buf, len + 1);
        if ((int) s != (int) (len + 1)) {
            printf("Error reading\n");
            return -1;
        }
        
        printf("%s\n", buf);
    }
    
    close(fd);
}
