#include "longint.h"
 
void test_empty_constructor() {
    puts("Testing empty constructor");
    longint l; 
    cout << "The resulting number:" << endl; 
    cout << l << endl; 
    puts("=== ===\n"); 
}

void test_int_constructor() {
    puts("Testing int constructor");
    int ourint = 2000000000; 
    longint l(ourint);
    cout << "The resulting number:" << endl; 
    cout << l << endl; 
    puts("=== ===\n"); 
}

void test_copy_constructor() {
    // to be changed 
    puts("Testing copy constructor");
    longint l("-89798729487897217878634");  
    cout << "l = " << l << endl; 
    longint p = l; 
    cout << "p = " << p << endl; 
    puts("=== ===\n"); 
}

void test_addition() {
    puts("Testing addition"); 
    longint l = 1; 
    for (int i = 0; i < 120; i++) 
    {
        l = l + l; 
        if ((i+1) % 10 == 0)
            cout << "2^" << i+1 << " = " << l << endl; 
    }
    puts("=== ===\n"); 
}

void test_subtraction() {
    puts("Testing subtraction"); 

    longint l("9871987487"); 
    longint m("-982873"); 
    longint t = l - m; 
    cout << "l - m = " << t << endl;  

    cout << "now (l - m) + m = " << t + m << endl; 
    cout << "And it is equal to l: " << ((t+m)==l) << endl;
    puts("=== ===\n"); 
}

void test_multiplication() {
    puts("Testing multiplicaiton"); 
       
    longint l("9881798"); 
    longint m("-9188374"); 
    longint t = l * m; 
    cout << "l * m = " << t << endl;  

    puts("=== ===\n"); 
}

/* 
void test_find_q() {
    puts("Testing find_q"); 
       
    char lstring[256], mstring[256]; 
    cout << "l = "; 
    cin.getline(lstring, 256); 
    cout << "m = "; 
    cin.getline(mstring, 256); 
    longint l(lstring);
    longint m(mstring);  
    cout << "find_q(l, m) = " << find_q(l, m) << endl;  

    puts("=== ===\n"); 
}
*/

void test_division() {
    puts("Testing division"); 

    longint l("200000000000000"); 
    longint m("2"); 
    longint t = l / m; 
    cout << "l / m = " << t << endl;  

    cout << "now (l / m) * m = " << t * m << endl; 
    puts("=== ===\n"); 
}

int main() {
    //test_empty_constructor(); 

    //test_int_constructor(); 

    test_copy_constructor();

    //test_addition(); 

    //test_subtraction(); 

    //test_multiplication(); 

    //test_find_q(); 

    test_division(); 

   return 0;
}
