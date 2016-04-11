#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>

#include <inttypes.h>
#include <signal.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>

#include <cstdlib>
#include <iostream>
#include <algorithm>

using namespace std;

static int signalIgnoring(void);
static void loop(int ld);

int main(void) {
    const char        *host = "127.0.0.1";
    uint16_t           port = 8000;
    struct sockaddr_in addr;
    int                ld;
    int                on;

    if(signalIgnoring() != 0)
        return -1;

    memset(&addr, 0, sizeof addr);
    addr.sin_family = AF_INET;
    addr.sin_port   = htons(port);
    if(inet_pton(AF_INET, host, &addr.sin_addr) < 1) {
        fprintf(stderr, "Wrong host\n");
        return -1;
    }

    ld = socket(AF_INET, SOCK_STREAM, 0);
    if(ld == -1) {
        fprintf(stderr, "Can't create socket\n");
        return -1;
    }

    on = 1;
    if(setsockopt(ld, SOL_SOCKET, SO_REUSEADDR, &on, sizeof on) == -1) {
        fprintf(stderr, "Can't setsockopt\n");
        if(close(ld))
            fprintf(stderr, "Can't realese file descriptor\n");
        return -1;
    }

    if(bind(ld, (struct sockaddr *)&addr, sizeof addr) == -1) {
        fprintf(stderr, "Can't bind\n");
        if(close(ld))
            fprintf(stderr, "Can't realese file descriptor\n");
        return -1;
    }

    if(listen(ld, 5) == -1) {
        fprintf(stderr, "Can't listen\n");
        if(close(ld))
            fprintf(stderr, "Can't realese file descriptor\n");
        return -1;
    }

    loop(ld);

    if(close(ld)) {
        fprintf(stderr, "Can't realese file descriptor\n");
        return -1;
    }

    puts("Done.");

    return 0;
}

static void handler(int signo);

static int signalIgnoring(void) {
    struct sigaction act;
    act.sa_handler = handler;
    act.sa_flags   = 0;
    sigemptyset(&act.sa_mask);

    if(sigaction(SIGINT, &act, 0) == -1) {
        fprintf(stderr, "Can't setup SIGINT ignoring\n");
        return -1;
    }

    if(sigaction(SIGPIPE, &act, 0) == -1) {
        fprintf(stderr, "Can't setup SIGPIPE ignoring\n");
        return -1;
    }

    return 0;
}

static void handler(int signo) {
    (void)signo;
}

static void loop(int ld) {
    struct sockaddr_in addr;
    socklen_t          addrlen;
    int                fd;

    uint32_t sz;
    //sz = sizeof(double) * 3;

    try {
        memset(&addr, 0, sizeof addr);
        addrlen = sizeof addr;

        fd = accept(ld, (struct sockaddr *)&addr, &addrlen);
        if(fd == -1) {
            if(errno == EINTR)
                return;
            else {
                fprintf(stderr, "Can't accept\n");
                //continue;
            }
        }

        if(read(fd, &sz, sizeof sz) != sizeof sz) {
            fprintf(stderr, "Can't read a length\n");
            throw -1;
        }

        double* res_arr = new double[sz / sizeof(double)];

        if(read(fd, res_arr, sz) != (ssize_t) sz) {
            fprintf(stderr, "Can't read the array\n");
            throw -1;
        }

        sort(res_arr, res_arr + sz / sizeof(double));

        if(write(fd, &sz, sizeof sz) != sizeof sz) {
            fprintf(stderr, "Can't write a length\n");
            throw -1;
        }

        if(write(fd, res_arr, sz) != (ssize_t) sz) {
            fprintf(stderr, "Can't write a text\n");
            throw -1;
        }
    } catch (...) {
            if(shutdown(fd, 2) == -1)
                fprintf(stderr, "Can't shutdown socket\n");

            if(close(fd))
                fprintf(stderr, "Can't realese file descriptor\n");
    }
}
