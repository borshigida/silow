#ifndef _LONGINT_H
#define _LONGINT_H
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <cmath>
#include <iomanip>
#include <climits>
#include <cerrno>

using namespace std;

const int base = 1000*1000*1000; 

class longint {
    unsigned int* _begin;
    unsigned int* _end; 
    bool _sign; 
public: 
    longint();

    longint(const longint& num); // copy constructor
    longint& operator=(const longint& a); // copy assignment

    longint (const int &num); // "int" constructor
    longint (const char* string); // "char*" constructor
    
    longint(size_t n); // "size_t" constructor 
    
    ~longint() { delete[] _begin; };

    size_t size() const { return _end-_begin; }

    unsigned int* begin() const { return _begin;  }
    unsigned int* end() const { return _end; }
    bool getsign() const { return _sign; }
    bool empty() const { return size()==0; }
    bool iszero() const;
    void changesign(); 
    longint getabs() const; 

    longint& operator++()
    {
        *this = *this + 1; 
        return *this; 
    }

    friend ostream &operator << (std::ostream &out, const longint& a) {
        if (a.getsign() == true) 
            out << '-';
        out << (a.empty() ? 0 : a.begin()[a.size()-1]);
        for (int i=(int)a.size()-2; i>=0; --i) 
            out << setfill('0') << setw(9) << a._begin[i];
        return out; 
    }

    friend longint operator-(const longint&); 
    friend bool operator==(const longint&, const longint&); 
    friend bool operator!=(const longint&, const longint&); 
    friend bool operator>=(const longint&, const longint&); 
    friend bool operator<=(const longint&, const longint&); 
    friend bool operator>(const longint&, const longint&); 
    friend bool operator<(const longint&, const longint&); 
    friend longint operator+(const longint&, const longint&); 
    friend longint operator-(const longint&, const longint&); 
    friend longint operator*(const longint&, const longint&); 
    friend int find_q(const longint&, const longint&); // temporary!!! 
    friend longint operator/(const longint&, const longint&); 

    void delete_leading_zeros(); 
};

#endif /* _LONGINT_H */ 
