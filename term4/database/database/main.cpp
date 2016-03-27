#include <unistd.h>
#include <fcntl.h>
#include <cstring>

#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <algorithm>
#include <vector>
#include <map>

using namespace std;

const int number_of_fields = 4;
const int field_lens[number_of_fields] = {20, 35, 20, 20};
const char* field_names[number_of_fields] = {"USER ID", "ITEM", "PRICE", "DATE"};

//const int MAXN = 100000; // the maximum number of entries in a database
const int MAXDATESIZE = 10;
const int MAXCOMMANDSIZE = 100;

const char* FARPAST = "0.0.0";
const char* FARFUTURE = "30.12.3000";


struct entry {
    int user_id;
    char *item;
    double price;
    char* date;
    
public:
    
    entry() {
        user_id= 0;
        item = new char[20];
        price = 0.0;
        date = new char[MAXDATESIZE];
    }
};

struct compare_dates {
    bool operator()(const char* date1, const char* date2) const {
        int day1, month1, year1, day2, month2, year2;
        sscanf(date1, "%d.%d.%d", &day1, &month1, &year1);
        sscanf(date2, "%d.%d.%d", &day2, &month2, &year2);
        if (year1 != year2) return (year1 < year2);
        if (month1 != month2) return (month1 < month2);
        if (day1 != day2) return (day1 < day2);
        return false;
    }
};

typedef multimap<const char*, entry, compare_dates> user_structure;
typedef map<int, user_structure> data_structure;

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

int read_entry(int fd, char* buf, entry& res) {
    // Reads one entry from fd (if possible), res becomes that entry.
    
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
        if (i == 0) { // user_id
            res.user_id = atoi(buf);
        } else if (i == 1) { // item
            strcpy(res.item, buf);
        } else if (i == 2) { // price
            res.price = atof(buf);
        } else if (i == 3) { // date
            strcpy(res.date, buf);
        }
    }
    return 0;
}

int write_entry(int fd, entry& theentry) {
    char* c_user_id = new char[20];
    char* c_price = new char[100];
    sprintf(c_user_id, "%d", theentry.user_id);
    sprintf(c_price, "%f", theentry.price);
    if (writef(fd, c_user_id) == -1) {
        return -1;
    }
    if (writef(fd, theentry.item) == -1) {
        return -1;
    }
    if (writef(fd, c_price) == -1) {
        return -1;
    }
    if (writef(fd, theentry.date) == -1) {
        return -1;
    }
    return 0;
}

int write_all_entries(int fd, data_structure& database) {
    for(data_structure::iterator it = database.begin(); it != database.end(); it++) {
        for(user_structure::iterator it2 = it->second.begin(); it2 != it->second.end(); it2++) {
            if (write_entry(fd, it2->second) != 0) {
                cerr << "Couldn't write all entries\n";
                return -1;
            }
        }
    }
    return 0;
}

int print_entry(entry& the_entry) {
    printf("   ");
    
    int num_characters;
    
    // printing user_id
    num_characters = printf("%d", the_entry.user_id);
    for (int k = 0; k < field_lens[0] - 3 - num_characters; k++) {
        printf(" ");
    }
    printf("   ");
    
    // printing item
    num_characters = printf("%s", the_entry.item);
    for (int k = 0; k < field_lens[1] - 3 - num_characters; k++) {
        printf(" ");
    }
    printf("   ");
    
    // printing price
    num_characters = printf("%f", the_entry.price);
    for (int k = 0; k < field_lens[2] - 3 - num_characters; k++) {
        printf(" ");
    }
    printf("   ");
    
    // printing date
    num_characters = printf("%s", the_entry.date);
    for (int k = 0; k < field_lens[3] - 3 - num_characters; k++) {
        printf(" ");
    }
    printf("   ");
    
    printf("\n");
    return 0;
}

int print_all_entries(data_structure& database) {
    for(data_structure::iterator it = database.begin(); it != database.end(); it++) {
        for(user_structure::iterator it2 = it->second.begin(); it2 != it->second.end(); it2++) {
            if (print_entry(it2->second) != 0) {
                cerr << "Couldn't print all entries\n";
                return -1;
            }
        }
    }
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

void work_select(data_structure& database, const int user = -1, const char* date1 = FARPAST, const char* date2 = FARFUTURE) {
    if (user == -1) {
        for (data_structure::iterator it = database.begin(); it != database.end(); it++) {
            user_structure& usermap = it->second;
            user_structure::iterator itlow, itup;
            itlow = usermap.lower_bound(date1);
            itup = usermap.upper_bound(date2);
            for (user_structure::iterator it = itlow; it != itup; it++) {
                print_entry(it->second);
            }
        }
        return;
    }
    user_structure& usermap = database[user];
    user_structure::iterator itlow, itup;
    itlow = usermap.lower_bound(date1);
    itup = usermap.upper_bound(date2);
    for (user_structure::iterator it = itlow; it != itup; it++) {
        print_entry(it->second);
    }
}

void work_delete(data_structure& database, const int user = -1, const char* date1 = FARPAST, const char* date2 = FARFUTURE) {
    if (user == -1) {
        for (data_structure::iterator it = database.begin(); it != database.end(); it++) {
            user_structure& usermap = it->second;
            user_structure::iterator itlow, itup;
            itlow = usermap.lower_bound(date1);
            itup = usermap.upper_bound(date2);
            for (user_structure::iterator it = itlow; it != itup;) {
                it = usermap.erase(it);
            }
        }
        return;
    }
    user_structure& usermap = database[user];
    user_structure::iterator itlow, itup;
    itlow = usermap.lower_bound(date1);
    itup = usermap.upper_bound(date2);
    for (user_structure::iterator it = itlow; it != itup;) {
        it = usermap.erase(it);
    }
}

int work(data_structure& database, char* command) {
    // execute a command
    int user;
    char* date1 = new char[MAXDATESIZE]; char* date2 = new char[MAXDATESIZE];
    
    int n;
    if (sscanf(command, "select%n", &n) == 0 && n == strlen("select")) {
        if (sscanf(command, "select %n", &n) == 0 && n == strlen(command)) {
            work_select(database);
        }
        else if (sscanf(command, "select%*[ ]user = %d %n", &user, &n) == 1 && n  == strlen(command)) {
            work_select(database, user);
        }
        else if (sscanf(command, "select%*[ ]user = %d%*[ ]period = [ %[.0-9], %[.0-9] ] %n", &user, date1, date2, &n) == 3 && n  == strlen(command)) {
            work_select(database, user, date1, date2);
        }
        else if (sscanf(command, "select%*[ ]period = [ %[.0-9], %[.0-9] ] %n", date1, date2, &n) == 2 && n  == strlen(command)) {
            work_select(database, -1, date1, date2);
        }
        else if (sscanf(command, "select%*[ ]period = [ %[.0-9], %[.0-9] ]%*[ ]user = %d %n", date1, date2, &user, &n) == 3 && n  == strlen(command)) {
            work_select(database, user, date1, date2);
        }
    }
    else if (sscanf(command, "delete%n", &n) == 0 && n == strlen("delete")) {
        if (sscanf(command, "delete %n", &n) == 0 && n == strlen(command)) {
            work_delete(database);
        }
        else if (sscanf(command, "delete%*[ ]user = %d %n", &user, &n) == 1 && n  == strlen(command)) {
            work_delete(database, user);
        }
        else if (sscanf(command, "delete%*[ ]user = %d%*[ ]period = [ %[.0-9], %[.0-9] ] %n", &user, date1, date2, &n) == 3 && n  == strlen(command)) {
            work_delete(database, user, date1, date2);
        }
        else if (sscanf(command, "delete%*[ ]period = [ %[.0-9], %[.0-9] ] %n", date1, date2, &n) == 2 && n  == strlen(command)) {
            work_delete(database, -1, date1, date2);
        }
        else if (sscanf(command, "delete%*[ ]period = [ %[.0-9], %[.0-9] ]%*[ ]user = %d %n", date1, date2, &user, &n) == 3 && n  == strlen(command)) {
            work_delete(database, user, date1, date2);
        }
    }
    return 0;
}

int main(int argc, char* argv[]) { // One argument: the name of the file from which we read random entries
    
    // === Opening files, checking the number of arguments ===
    if (argc != 2) {
        cerr << "The number of arguments is wrong\n";
        exit(-1);
    }
    
    int fd = open(argv[1], O_RDONLY);
    
    if (fd == -1) {
        cerr << "Error opening file\n";
        exit(-1);
    }
    // === ===
    
    
    // === Creating the map ===
    char* buf = new char[1024];
    data_structure database; // main database map
    
    while(true) {
        entry res_entry;
        int read_res = read_entry(fd, buf, res_entry);
        if (read_res == -1) {
            cerr << "Error reading\n";
            exit(-1);
        }
        else if (read_res == 1) {
            break;
        }
        
        database[res_entry.user_id].insert(pair<const char*, entry>(res_entry.date, res_entry));
    }
    
    for(data_structure::iterator it = database.begin(); it != database.end(); it++) {
        for(user_structure::iterator it2 = it->second.begin(); it2 != it->second.end(); it2++) {
            print_entry(it2->second);
        }
    }
    // ==== ===
    
    close(fd);
    
    fd = open(argv[1], O_CREAT | O_WRONLY | O_TRUNC, S_IRUSR | S_IWUSR);
    // === Asking for the command ===
    string line;
    char* command = new char[MAXCOMMANDSIZE];
    
    printf("> ");
    
    getline(cin, line);
    strcpy(command, line.c_str());
    
    work(database, command);
    
    write_all_entries(fd, database);
    // === ===
    
    close(fd);
}
