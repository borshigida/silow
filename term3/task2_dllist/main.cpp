#include "longint.h"
#include "dllist.h"

// === TEST ===
template<typename _t>
void test_empty_constructor()
{
    puts("=== Testing empty constructor ===");
    dllist<_t> a;
    assert(a.begin() == a.end());
    assert(a.size() == 0);
    puts("=== ===\n\n");
}

template<typename _t>
void test_size_constructor()
{
    puts("=== Testing size constructor ===");
    const size_t size = 5;
    
    dllist<_t> x(size);
    cout << x;
    
    assert(x.size() == size);
    
    puts("=== ===\n\n");
}

template<typename _t>
void test_size_val_constructor()
{
    puts("=== Testing size-val constructor ===");
    const size_t size = 5;
    const int val = 51; // !
    
    dllist<_t> x(size, val);
    cout << x;
    
    assert(x.size() == size);
    
    puts("=== ===\n\n");
}

template<typename _t>
void test_copy_constructor()
{
    puts("=== Testing copy constructor ===");
    const size_t size = 5;
    const longint val = 51 * longint(100000000) * longint("97984798798701987874687681797947419879871897") * longint(100000000); // !
    
    dllist<_t> x(size, val);
    cout << x;
    
    dllist<_t> y(x);
    cout << y;
    
    assert(x.size() == y.size());
    typename dllist<_t>::iterator it1 = x.begin();
    typename dllist<_t>::iterator it2 = y.begin();
    for(int i = 0; i < x.size(); ++i, ++it1, ++it2)
        assert(*it1 == *it2);
    
    puts("=== ===\n\n");
}

template<typename _t>
void test_copy_assignment()
{
    puts("=== Testing copy assignment ===");
    const size_t size = 5;
    const size_t size2 = 3;
    const size_t size3 = 9;
    
    dllist<_t> x(size);
    cout << x;
    
    int i = 0;
    for (typename dllist<_t>::iterator it = x.begin(); it != x.end(); ++it, ++i)
        *it = i;
    
    cout << x;
    
    dllist<_t> y(size2);
    y = x;
    cout << y;
    
    dllist<_t> z(size3);
    z = y;
    cout << z;
    
    assert(x.size() == z.size());
    typename dllist<_t>::iterator it1 = x.begin();
    typename dllist<_t>::iterator it2 = z.begin();
    for(int i = 0; i < x.size(); ++i, ++it1, ++it2)
        assert(*it1 == *it2);
    
    puts("=== ===\n\n");
}

template<typename _t>
void test_insert()
{
    puts("=== Testing insert ===");
    const size_t size = 5;
    const int val = 51;
    
    dllist<_t> x(size, val);
    cout << x;
    
    typename dllist<_t>::iterator it = x.begin();
    ++it;
    const longint many = longint(100000000) * longint(100000000) * longint(100000000); 
    for(int i = 0; i < 3; ++i, ++it)
    {
        it = x.insert(it, many + i);
    }
    
    cout << x;
    
    puts("=== ===\n\n");
}

template<typename _t>
void test_erase()
{
    puts("=== Testing erase ===");
    const size_t size = 6;
    const size_t pos = 2;
    
    dllist<_t> x(size);
    longint i = longint(100000000) * longint(100000000) * longint(100000000);
    for (typename dllist<_t>::iterator it = x.begin(); it != x.end(); ++it, ++i)
        *it = i;
    cout << x;
    
    i = 0;
    typename dllist<_t>::iterator it;
    for(it = x.begin(); i < longint(int(pos)); ++i, ++it)
        ;
    size_t tmp_size = x.size();
    x.erase(it);
    assert(x.size() == tmp_size - 1);
    
    cout << x;
    
    puts("=== ===\n\n");
}



int main()
{
    typedef longint _t;
    
    test_empty_constructor<_t>();
    test_size_constructor<_t>();
    test_size_val_constructor<_t>();
    test_copy_constructor<_t>();
    test_copy_assignment<_t>();
    test_insert<_t>();
    test_erase<_t>();
    return 0;
}
