 
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>

#include <string.h>
#include <stdio.h>
#include <cstdlib>
#include <algorithm>
#include <cstdio>
#include <iostream>

using namespace std;


static void writeRead(int fd, int arr_len, double* arr) {
    uint32_t sz;
    //char     buf[1024];

    sz = arr_len * sizeof(double);

    if(write(fd, &sz, sizeof sz) != sizeof sz) {
        fprintf(stderr, "Can't write array size\n");
        return;
    }

    if(write(fd, arr, sz) != (ssize_t) sz) {
        fprintf(stderr, "Can't write array\n");
        return;
    }

    if(read(fd, &sz, sizeof sz) != sizeof sz) {
        fprintf(stderr, "Can't read array size\n");
        return;
    }

    if(read(fd, arr, sz) != (ssize_t) sz) {
        fprintf(stderr, "Can't read array\n");
        return;
    }

    for (int i = 0; i < arr_len; i++) {
        cout << arr[i] << " ";
    }
    cerr << endl;

}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        cerr << "The number of arguments is wrong\n";
        return -1;
    }

    const char        *host = "127.0.0.1";
    uint16_t           port = 8000;
    struct sockaddr_in addr;
    int                fd;

    memset(&addr, 0, sizeof addr);
    addr.sin_family = AF_INET;
    addr.sin_port   = htons(port);
    if(inet_pton(AF_INET, host, &addr.sin_addr) < 1) {
        fprintf(stderr, "Wrong host\n");
        return -1;
    }

    fd = socket(AF_INET, SOCK_STREAM, 0);
    if(fd == -1) {
        fprintf(stderr, "Can't create socket\n");
        return -1;
    }

    if(connect(fd, (struct sockaddr *)&addr, sizeof addr) == -1) {
        fprintf(stderr, "Can't connect to the server\n");
        if(close(fd))
            fprintf(stderr, "Can't realese file descriptor\n");
        return -1;
    }

// ==============

    double* arr = new double[1024];
    int len = 0;
    for (int n = 0;; len++) {
        //cerr << len << endl;
        int newn;
        if (sscanf(argv[1] + n, "%lf %n", &arr[len], &newn) <= 0) {
            break;
        }
        n += newn;
    }

    for (int i = 0; i < len; i++) {
        cerr << arr[i] << " ";
    }
    cerr << endl;
    writeRead(fd, len, arr);

// ==============



    if(shutdown(fd, 2) == -1)
        fprintf(stderr, "Can't shutdown socket\n");

    if(close(fd)) {
        fprintf(stderr, "Can't realese file descriptor\n");
        return -1;
    }

    return 0;
}

