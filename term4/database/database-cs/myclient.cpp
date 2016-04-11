 
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
	char buf[1024]; 	
    
	if(read(fd, &len, sizeof len) != sizeof len) {
		fprintf(stderr, "Can't read length\n"); 
		return; 
	}

	if (len > sizeof buf) {
		fprintf(stderr, "Too big message\n"); 
		return;
	}

	if (read(fd, buf, len) != (ssize_t) len) {
		fprintf(stderr, "Can't read text\n"); 
		return; 
	}
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

    // === Opening files, checking the number of arguments ===
    if (argc != 2) {
        cerr << "The number of arguments is wrong\n";
        return -1;
    }
    
    data_structure database;
    load(argv[1], database);
    
    // === Executing commands ===
    string line;
    char* command = new char[MAXCOMMANDSIZE];
    
    while(true) {
        printf("> ");
        
        getline(cin, line);
        strcpy(command, line.c_str());

        //cout << "your command" << command << "\n";
        
        int ans = work(argv[1], database, command);
        //printf("interesting string\n");
        cout << endl;
        //cerr << ans << endl;
        if (ans == -1) return -1;
        if (ans == 2) {
            return 0;
        }
    }
    // === ===

// === ===

    if(shutdown(fd, 2) == -1)
        fprintf(stderr, "Can't shutdown socket\n");

    if(close(fd)) {
        fprintf(stderr, "Can't realese file descriptor\n");
        return -1;
    }

    return 0;
}

