#include "database.h"

int work_select(data_structure& database, const int user = -1, const char* date1 = FARPAST, const char* date2 = FARFUTURE) {
    char* entry_string = new char[1024];
    print_title(entry_string);
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
        return 0;
    }
    user_structure& usermap = database[user];
    user_structure::iterator itlow, itup;
    itlow = usermap.lower_bound(date1);
    itup = usermap.upper_bound(date2);
    for (user_structure::iterator it = itlow; it != itup; it++) {
        print_entry(it->second);
    }
    return 0;
}

int work_delete(data_structure& database, const int user = -1, const char* date1 = FARPAST, const char* date2 = FARFUTURE) {
    if (user == -1) {
        for (data_structure::iterator it = database.begin(); it != database.end(); it++) {
            user_structure& usermap = it->second;
            user_structure::iterator itlow, itup;
            itlow = usermap.lower_bound(date1);
            itup = usermap.upper_bound(date2);
            for (user_structure::iterator it = itlow; it != itup;) {
                try {
                    usermap.erase(it++);
                } catch (...) {
                    return -1;
                }
            }
        }
        return 0;
    }
    user_structure& usermap = database[user];
    user_structure::iterator itlow, itup;
    itlow = usermap.lower_bound(date1);
    itup = usermap.upper_bound(date2);
    for (user_structure::iterator it = itlow; it != itup;) {
        try {
            usermap.erase(it++);
        } catch (...) {
            return -1;
        }
    }
    return 0;
}

int load(const char* filename, data_structure& database) {
    database.clear();
    
    int fd = open(filename, O_RDONLY);
    
    if (fd == -1) {
        cerr << "Error opening file\n";
        return -1;
    }
    // === ===
    
    // === Creating the map ===
    char* buf = new char[1024];
    
    while(true) {
        entry res_entry;
        int read_res = read_entry(fd, buf, res_entry);
        if (read_res == -1) {
            cerr << "Error reading\n";
            close(fd);
            return -1;
        }
        else if (read_res == 1) {
            break;
        }
        
        database[res_entry.user_id].insert(pair<const char*, entry>(res_entry.date, res_entry));
    }
    // ==== ===
    
    close(fd);
    return 0;
}

int save(const char* filename, data_structure& database) {
    int fd = open(filename, O_CREAT | O_WRONLY | O_TRUNC, S_IRUSR | S_IWUSR);
    if (fd == -1) {
        cerr << "Error opening file\n";
        return -1;
    }
    if (write_all_entries(fd, database) == -1) {
        close(fd);
        return -1;
    }
    close(fd);
    return 0;
}

int work(const char* filename, data_structure& database, char* command) {
    // execute a command
    int user;
    char* date1 = new char[MAXDATESIZE]; char* date2 = new char[MAXDATESIZE];
    int newuser; char* newitem = new char[1024]; char* newdate = new char[1024]; double newprice;
    
    int n;
    if (sscanf(command, "select%n", &n) == 0 && n == strlen("select")) {
        if (sscanf(command, "select %n", &n) == 0 && n == (int) strlen(command)) {
            work_select(database);
        }
        else if (sscanf(command, "select%*[ ]user = %d %n", &user, &n) == 1 && n  == (int) strlen(command)) {
            work_select(database, user);
        }
        else if (sscanf(command, "select%*[ ]user = %d%*[ ]period = [ %[.0-9], %[.0-9] ] %n", &user, date1, date2, &n) == 3 && n  == (int) strlen(command)) {
            work_select(database, user, date1, date2);
        }
        else if (sscanf(command, "select%*[ ]period = [ %[.0-9], %[.0-9] ] %n", date1, date2, &n) == 2 && n  == (int) strlen(command)) {
            work_select(database, -1, date1, date2);
        }
        else if (sscanf(command, "select%*[ ]period = [ %[.0-9], %[.0-9] ]%*[ ]user = %d %n", date1, date2, &user, &n) == 3 && n  == (int) strlen(command)) {
            work_select(database, user, date1, date2);
        }
    }
    else if (sscanf(command, "delete%n", &n) == 0 && n == (int) strlen("delete")) {
        if (sscanf(command, "delete %n", &n) == 0 && n == (int) strlen(command)) {
            if (work_delete(database) == -1) return -1;
        }
        else if (sscanf(command, "delete%*[ ]user = %d %n", &user, &n) == 1 && n  == (int) strlen(command)) {
            if (work_delete(database, user) == -1) return -1;
        }
        else if (sscanf(command, "delete%*[ ]user = %d%*[ ]period = [ %[.0-9], %[.0-9] ] %n", &user, date1, date2, &n) == 3 && n  == (int) strlen(command)) {
            if (work_delete(database, user, date1, date2) == -1) return -1;
        }
        else if (sscanf(command, "delete%*[ ]period = [ %[.0-9], %[.0-9] ] %n", date1, date2, &n) == 2 && n  == (int) strlen(command)) {
            if (work_delete(database, -1, date1, date2) == -1) return -1;
        }
        else if (sscanf(command, "delete%*[ ]period = [ %[.0-9], %[.0-9] ]%*[ ]user = %d %n", date1, date2, &user, &n) == 3 && n  == (int) strlen(command)) {
            if (work_delete(database, user, date1, date2) == -1) return -1;
        }
    }
    else if (sscanf(command, "insert %d [%[a-zA-Z ]] %lf %[.0-9] %n", &newuser, newitem, &newprice, newdate, &n) == 4 && n == (int) strlen(command)) {
        entry newentry = entry();
        newentry.user_id = newuser;
        strcpy(newentry.item, newitem);
        newentry.price = newprice;
        strcpy(newentry.date, newdate);
        database[newuser].insert(pair<const char*, entry>(newdate, newentry));
    }
    else if (sscanf(command, "save %n", &n) == 0 && n == (int) strlen(command)) {
        if (save(filename, database) == -1) return -1;
    }
    else if (sscanf(command, "load %n", &n) == 0 && n == (int) strlen(command)) {
        if (load(filename, database) == -1) return -1;
    }
    else if (sscanf(command, "exit %n", &n) == 0 && n == (int) strlen(command)) {
        return 2;
    }
    else {
        printf("Sorry, didn't understand\n");
    }
    return 0;
}

int main(int argc, char* argv[]) { // One argument: the name of the file from which we read random entries
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
        int ans = work(argv[1], database, command);
        cout << endl;
        if (ans == -1) return -1;
        if (ans == 2) return 0;
    }
    // === ===
}
