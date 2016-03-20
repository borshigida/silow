#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>

const int number_of_fields = 4;
const int field_lens[number_of_fields] = {20, 35, 20, 20};
const char* field_names[number_of_fields] = {"USER ID", "ITEM", "PRICE", "DATE"};

struct entry {
    int user_id;
    char *item;
    double price;
    char* date;
};

int readf(int fd, char* buf) {
    size_t len;
    ssize_t s;

    s = read(fd, &len, sizeof len);
    if (s == 0)
        return 1;
    if (s != sizeof len) {
        printf("Error reading\n");
        return -1;
    }
    s = read(fd, buf, len + 1);
    if ((int) s != (int) (len + 1)) {
        printf("Error reading\n");
        return -1;
    }

    //printf("(buf = %s)\n", buf);
    return 0;
}

void empty_string(char* res_string) {
    strcat(res_string, "   ");
    for (int i = 0; i < number_of_fields; i++) {
        for (int k = 0; k < field_lens[i] - 3; k++) {
            strcat(res_string, " ");
        }
        strcat(res_string, "   ");
    }
}

void title_string(char* res_string) {
    strcat(res_string, "   ");
    for (int i = 0; i < number_of_fields; i++) {
        strcat(res_string, field_names[i]);
        for (int k = 0; k < field_lens[i] - 3 - (int) strlen(field_names[i]); k++) {
            strcat(res_string, " ");
        }
        strcat(res_string, "   ");
    }
}

void print_title(char* entry_string) {
    memset(entry_string, '\0', strlen(entry_string));
    empty_string(entry_string);
    printf("%s\n", entry_string);

    memset(entry_string, '\0', strlen(entry_string));
    title_string(entry_string);
    printf("%s\n", entry_string);

    memset(entry_string, '\0', strlen(entry_string));
    empty_string(entry_string);
    printf("%s\n", entry_string);
}

int readf_whole_entry(int fd, char* buf, char* res_string) {
    strcat(res_string, "   ");
    for (int i = 0; i < number_of_fields; i++) {
        int read_res = readf(fd, buf);
        if (read_res == -1) {
            return -1;
        }
        else if (read_res == 1 && i != 0) {
            return -1; // the end of the file in the middle of an entry
        }
        else if (read_res == 1 && i == 0) {
            return 1; // the end of the file, can't read anything
        }
        strcat(res_string, buf);
        for (int k = 0; k < field_lens[i] - 3 - (int) strlen(buf); k++) {
            strcat(res_string, " ");
        }
        strcat(res_string, "   ");
    }
    return 0;
}

int main(int argc, char* argv[]) {
    // One argument: the name of the file from which we read random entries

    int fd = open(argv[1], O_RDONLY);

    if (fd == -1) {
        printf("Error opening file\n");
        return -1;
    }

    if (argc > 2) {
        printf("Too many arguments\n");
        return -1;
    }

    char* buf = new char[1024];
    char* entry_string = new char[1024];

    print_title(entry_string);
    while(true) {
        memset(entry_string, '\0', strlen(entry_string));
        int read_res = readf_whole_entry(fd, buf, entry_string);
        if (read_res == -1) {
            printf("Error reading\n");
            return -1;
        }
        else if (read_res == 1) {
            break;
        }
        printf("%s\n", entry_string);
    }

    close(fd);
}
