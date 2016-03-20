#include "longint.h"

int toInt (const char *s, int& x) 
{
    long l;
    char *e;
    errno = 0;
    l = strtol(s, &e, 10);
    if (!errno && *e == '\0' && INT_MIN <= l && l <= INT_MAX) {
        x = (int) l;
        return 0;
    } 
    else 
        return -1;
}

longint::longint() // empty constructor initializes zero
{
    _begin = new unsigned int[1]; 
    _begin[0] = 0;
    _end = _begin+1;
    _sign = false;     
}

longint::longint(const longint& num)  // copy constructor
{
    _begin = new unsigned int[num.size()]; 

    for (int i=0; i<(int)num.size(); ++i)
        _begin[i] = num.begin()[i]; 
    _end = _begin+num.size(); 
    _sign = num.getsign(); 
}

longint& longint::operator=(const longint& num)  // copy assignment 
{
    unsigned int* p = new unsigned int[num.size()]; 
    for (int i=0; i<(int)num.size(); ++i)
        p[i] = num.begin()[i]; 
    delete[] _begin; 
    _begin = p; 
    _end = _begin+num.size(); 
    _sign = num.getsign();

    return *this;
}

longint::longint(const int& num)  // "int" constructor
{
    _begin = new unsigned int[2]; 
    _begin[0] = abs(num) % base;
    _begin[1] = abs(num) / base; 
    _sign = (num<0) ? true : false; 
    _end = _begin+2;
    delete_leading_zeros();     
}

longint::longint(size_t n) // "size_t" constructor
{
    _begin = new unsigned int[n]; 
    _end = _begin+n; 
    _sign = false; 
    for(unsigned int* p = _begin; p!=_end; ++p) 
        *p = 0; 
}

longint::longint(const char* s) // "char*" constructor
{
    try
    {
        char string[256]; 
        strcpy(string, s); 

        for(int i=(int)strlen(string)-1; i>=0; --i) {
            if (isspace(string[i]))
                string[i] = 0;
        }

        bool newsign = false; 
        if (string[0]=='-')
        {
            newsign = true;
        }
            
        size_t newsize = (size_t) ((int) strlen(string))/9 + 2; 
        _begin = new unsigned int[newsize]; 
        _end = _begin+newsize; 
        _sign=false; 
        int elnum = 0; 
        for (int i=(int)strlen(string); i>(newsign ? 1 : 0); i-=9, ++elnum)
        {
            // cerr << "*this now: " << *this << endl; 
            string[i] = 0; 
            int x=0; 
            if (i>=9)
            {
                if (toInt(string+i-9, x)==0)
                {
                    _begin[elnum] = x; 
                    //cerr << "x now: " << x << endl; 
                }
                else
                {
                    throw 1; 
                }
            }
            else if (i<9)
            {
                if (toInt(string+(newsign ? 1 : 0), x)==0)
                {
                    _begin[elnum] = x; 
                    //cerr << "x now: " << x << endl; 
                }
                else
                    throw 1;
            }
        }

        _end = _begin+elnum; 

        delete_leading_zeros(); 

        if (newsign==true) changesign(); 
    }
    catch(int e)
    {
        cout << "Exception type " << e << endl; 
        exit(0); 
    }
}

bool longint::iszero() const 
{
    for (int i=0; i<(int)size(); i++)
        if (_begin[i]!=0)
            return false; 
    return true; 
}

void longint::changesign() 
{
    if (!(iszero()))
        _sign = !_sign; 
}

longint longint::getabs() const 
{
    return (_sign==false) ? (*this) : -(*this); 
}

void longint::delete_leading_zeros() {
    unsigned int* p; 
    for(p = _end-1; (*p)==0 && p!=_begin; --p) 
        ; 
    _end = p+1; 
}

longint operator-(const longint& a) {
    longint temp = a;
    temp.changesign(); 
    return temp; 
}

bool operator==(const longint& a, const longint& b) 
{
    if (a.getsign()!=b.getsign()) 
        return false; 
    if (a.size()!=b.size()) 
        return false; 
    for (int i=0; i < a.size(); ++i)
        if (a.begin()[i] != b.begin()[i])
            return false; 
    return true; 
}

bool operator!=(const longint& a, const longint& b) 
{
    return !(a==b); 
}

bool operator>(const longint& a, const longint& b) 
{
    if (a.getsign()==false && b.getsign()==true)
        return true; 
    else if (a.getsign()==true && b.getsign()==false) 
        return false; 
    if (a.getsign()==true) return (a.getabs() < b.getabs());
    if (a.size()!=b.size()) return (a.size() > b.size());
    for(int i=(int)a.size()-1; i>=0; --i) 
        if (a.begin()[i] != b.begin()[i])
            return (a.begin()[i] > b.begin()[i]); 
    return false; 
}

bool operator<(const longint& a, const longint& b) 
{
    return !(a>b || a==b); 
}

bool operator>=(const longint& a, const longint& b) 
{
    return (a>b || a==b); 
}

bool operator<=(const longint& a, const longint& b) 
{
    return (a<b || a==b); 
}

longint operator+(const longint& a, const longint& b) 
{
    if (a.getsign() == false && b.getsign() == true)
        return (a-b.getabs()); 
    if (a.getsign() == true && b.getsign() == false) 
        return (b-a.getabs()); 
    // now a._sign == b._sign

    longint res(max(a.size(), b.size())+1); 

    // cerr << "res size: " << res.size() << endl; 
    int carry=0; 
    for (size_t i=0; i < max(a.size(), b.size()) || carry; ++i) {
        // cerr << "i: " << i << endl; 
        int digit = carry + (i < a.size() ? a.begin()[i] : 0) + (i < b.size() ? b.begin()[i] : 0); 
        // cerr << "base: " << base << ", digit: " << digit << endl; 
        carry = digit >= base; 
        if (carry) digit -= base; 
        res.begin()[i] = digit; 
        // cerr << "i: " << i << endl; 
    }

    res.delete_leading_zeros(); 

    if (a.getsign() == true)
        res.changesign(); 

    return res; 
}

longint operator-(const longint& a, const longint& b) 
{
    if (a.getsign() == false && b.getsign() == true)
        return (a+b.getabs()); 
    if (a.getsign() == true && b.getsign() == false)
        return -(a.getabs() + b); 
    if (a.getabs()<b.getabs())
        return -(b-a); 
    // abs(a) >= abs(b)
    longint res(a.size()); 

    int carry=0; 
    for (size_t i=0; i<a.size(); ++i) {
        int digit = a.begin()[i] - carry - (i < b.size() ? b.begin()[i] : 0); 
        carry = (digit < 0); 
        if (carry) digit += base; 
        res.begin()[i] = digit; 
    }

    res.delete_leading_zeros(); 

    if (a.getsign() == true) res.changesign(); 
    return res; 
}

longint operator*(const longint& a, const longint& b)
{
    longint res(a.size() + b.size() + 2); 
    int carry = 0; 
    for (size_t i=0; i<a.size(); ++i) {
        carry = 0; 
        for (size_t j=0; j<b.size() || carry; ++j) {
            long long digit = res.begin()[i+j] + carry + (i < a.size() ? a.begin()[i] : 0) * 1ll * (j < b.size() ? b.begin()[j] : 0); 
            carry = int (digit / base); 
            res.begin()[i+j] = int (digit % base); 
        }
    }

    res.delete_leading_zeros(); 

    if (a.getsign() != b.getsign()) res.changesign(); 
    return res; 
}


int find_q(const longint& a, const longint& b) 
{
    // here a.size() - b.size() == 0 or 1 
    int l = 0; 
    int r = base; 
    while (r - l != 1)
    {
        int m = (l + r) / 2; 
        if (b * longint(m) <= a)
            l = m; 
        else
            r = m; 
    }
    return l; 
}

longint operator/(const longint& a, const longint& b)
{
    if (b==0)
        cerr << "very bad" << endl; 
    if (a.getabs() < b.getabs())
        return 0; 
    if (a.getsign() != b.getsign())
        return -(a.getabs()/b.getabs()); 
    
    unsigned int* reversed_res = new unsigned int[a.size() - b.size() + 4]; 
//    cerr << "new successful" << endl; 

    // initialization of cur 
    longint cur((size_t) b.size()); 
    for (size_t i=0; i < b.size(); ++i)
        cur.begin()[i] = a.begin()[a.size()-b.size()+i];
    
    // the division  
    cerr << "before division successful" << endl; 
    int elnum = 0; 
//    cerr << "a.size() = " << a.size() << ", b.size() = " << b.size() << endl; 
    for(int j = (int) a.size() - (int) b.size() - 1; j >= -1;)
    {
//        cerr << "cur = " << cur << endl; 
        reversed_res[elnum] = find_q(cur, b); 
//        cerr << "findq = " << longint((int) reversed_res[elnum]) << endl; 
//        cerr << cur - longint((int) reversed_res[elnum]) * b;
        cur = cur - longint((int) reversed_res[elnum]) * b;
        ++elnum; 
//        cerr << "curr after subtraction = " << cur << endl; 
        if (cur<b) {
            if (j > -1)
                cur = cur * longint(base) + longint((int) a.begin()[j]);
            --j; 
        }
    }

//    cerr << "elnum now: " << elnum << endl; 

//    cerr << "printing reversed_res" << endl; 
//    for (int i=0; i<a.size() - b.size() + 4; i++)
//        cerr << reversed_res[i] << " "; 
//    cerr << endl; 
    longint res((size_t) elnum); 
    for(int i=0; i < elnum; ++i)
    {
//        cerr << "reversed_res[elnum-1-i] = " << reversed_res[elnum-1-i] << endl; 
        res.begin()[i] = reversed_res[elnum-1-i]; 
    }

//    cerr << "res now: " << res.begin()[0] << " " << res.begin()[1] << endl; 
    res.delete_leading_zeros(); 
    delete[] reversed_res; 
    return res;
}
