#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <algorithm>
#include <time.h>

const int USER_MAX = 10000;
const int PRICE_MAX = 1000000;
const size_t ITEM_MAX = 30;

const int NUMBER_OF_ITEMS = 9;
const char* ITEMS[NUMBER_OF_ITEMS] = {"Notebook", "Laptop", "Macbook", "Mouse", "Tape recorder", "MP3 Player", "Samsung Galaxy S6", "IPhone 50s", "North Korea Dictator"};

struct entry {
    int user_id;
    char *item;
    double price;
    char* date;
};

entry& generate_random_entry() {
    entry& res = *(new entry);
    res.user_id = rand() % USER_MAX + 1;

    /*
    const size_t itemlen = rand() % ITEM_MAX + 1;
    res.item = new char[itemlen];
    for (size_t i = 0; i < itemlen; i++) {
        res.item[i] = rand() % ('z' - 'a') + 'a';
    }
    */

    res.item = new char[ITEM_MAX];
    strcpy(res.item, ITEMS[rand() % NUMBER_OF_ITEMS]);

    res.price = rand() % (PRICE_MAX - 1) + rand() / RAND_MAX;
    res.date = new char[12];
    sprintf(res.date,"%d.%d.%d", rand() % 28 + 1, rand()  % 12 + 1, rand() % 200 + 1900);

    //printf("user_id: %d, item: %s, price: %f, date: %s\n", res.user_id, res.item, res.price, res.date);

    return res;
}


int writef(int fd, char* string) {
    size_t len = strlen(string);
    if (write(fd, &len, sizeof len) != sizeof len) {
        printf("Error writing\n");
        return -1;
    }
    if ((int) write(fd, string, len + 1) != (int) len + 1) {
        printf("Error writing\n");
        return -1;
    }
    return 0;
}


int main(int argc, char* argv[]) {
/*
Три аргумента, первый --- имя файла, второй --- количество рандомных записей.
*/
    if (argc != 3) {
        printf("The number of arguments is wrong\n");
        return -1;
    }
    int fd = open(argv[1], O_CREAT | O_WRONLY | O_TRUNC, S_IRUSR | S_IWUSR);

    if (fd == -1) {
        printf("Error opening file\n");
        return -1;
    }

    srand(time(NULL));
    int n = atoi(argv[2]); // количество рандомных записей
    char* c_user_id = new char[20];
    char* c_price = new char[100];
    for (int i = 0; i < n; i++) {
        entry& random_entry = generate_random_entry();
        sprintf(c_user_id,"%d", random_entry.user_id);
        sprintf(c_price,"%f", random_entry.price);
        if (writef(fd, c_user_id) == -1) {
            return -1;
        }
        if (writef(fd, random_entry.item) == -1) {
            return -1;
        }
        if (writef(fd, c_price) == -1) {
            return -1;
        }
        if (writef(fd, random_entry.date) == -1) {
            return -1;
        }
    }

    close(fd);
}
