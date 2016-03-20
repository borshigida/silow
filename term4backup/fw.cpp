#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>

int main(int argc, char* argv[]) {
    int fd = open(argv[1], O_CREAT | O_WRONLY | O_TRUNC, S_IRUSR | S_IWUSR);
    
    if (fd == -1) {
        printf("Error opening file\n");
        return -1;
    }
    
    for (int i = 2; i < argc; i++) {
        size_t len = strlen(argv[i]);
        if (write(fd, &len, sizeof len) != sizeof len) {
            printf("Error writing\n");
            return -1;
        }
        if ((int) write(fd, argv[i], len + 1) != (int) len + 1) {
            printf("Error writing\n");
            return -1;
        }
    }
    
    close(fd);
}
