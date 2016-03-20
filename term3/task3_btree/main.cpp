#include "btree.h"

void test_insertion()
{
    cout << "Testing insertion" << endl;
    btree<int, int, 2> a;
    for (int i = 0; i < 1000; i++)
        a[i] = 1000 - i;
    for (int i = 0; i < 1000; i++)
    {
        assert(a[i] == 1000 - i);
        cout << a[i] << ' ';
    }
    cout << endl << endl;
}

void test_deletion()
{
    cout << "Testing deletion" << endl;
    btree<int, int, 2> a;
    for (int i = 0; i < 20; i++)
        a[i] = 20 - i;
    for (int i = 0; i < 20; i++)
    {
        cout << a[i] << ' ';
        assert(a[i] == 20 - i);
    }
    cout << endl;
    a.del(1);
    for (int i = 0; i < 20; i++)
    {
        cout << a[i] << ' ';
        if (i != 1)
        {
            assert(a[i] == 20 - i);
        }
        else
        {
            assert(a[i] == 0);
        }
    }
    cout << endl << endl;
}

void test_both()
{
    cout << "Testing both" << endl;
    btree<int, int, 2> a;
    for (int i = 0; i < 10; i++)
        a[2*i] = 10 - i;
    for (int i = 1; i < 21; i += 2)
        a[i] = 13;
    cout << endl;
    for (int i = 0; i < 20; i++)
        cout << a[i] << ' ';
    cout << endl;
    
    for (int i = 0; i < 19; i+=4)
        a.del(i);
    
    for (int i = 0; i < 20; i++)
    {
        if (i % 4 == 0)
            assert(a[i] == 0);
        cout << a[i] << ' ';
    }
    cout << endl << endl;
}

void special_test()
{
    cout << "Special test" << endl;
    btree<int, int, 2> a;
    for (int i = 0; i < 10; i++)
        a[2*i] = 2*i;
    for (int i = 1; i < 21; i += 2)
        a[i] = i;
    cout << endl;
    assert(a.len() == 20);
    for (int i = 0; i < 20; i++)
        cout << a[i] << ' ';
    cout << endl;
    
    a.del(16);
    
    for (int i = 0; i < 20; i++)
    {
        cout << a[i] << ' ';
        if (i == 16)
            assert(a[i] == 0);
    }
    cout << endl << endl;
}

void test_copy_constructor()
{
    cout << "Testing copy constructor" << endl;
    btree<int, int, 2> a;
    for (int i = 0; i < 10; i++)
        a[2*i] = 2*i;
    for (int i = 1; i < 21; i += 2)
        a[i] = i;
    cout << endl;
    assert(a.len() == 20);
    for (int i = 0; i < 20; i++)
        cout << a[i] << ' ';
    cout << endl;
    
    btree<int, int, 2> b(a);
    
    for (int i = 0; i < 20; i++)
    {
        cout << b[i] << ' ';
    }
    cout << endl;
    b = a;
    for (int i = 0; i < 20; i++)
    {
        cout << b[i] << ' ';
        assert(b[i] == a[i]);
    }
    cout << endl << endl;
}

void hard_test(int n, int m)
{
    puts("=== Testing hard ===");
    btree<int, int, 4> a, b;
    for (int i = 0; i < n; ++i)
    {
        btree<int, int, 4> x, y;
        for (int j = 0; j <= m; ++j)
            x[j] = j;
        for (int j = m; j >= 0; --j)
            y[j] = j;
        btree<int, int, 4> u(x), v;
        
        v = y;
        x = x; y = y; u = u; v = v;
        a = u;
        b = v;
        
        for(int j = m; j > 10; --j)
        {
            a.del(j);
            b.del(j);
        }
    }
    for (int i = 0; i < 11; i++)
    {
        cout << a[i] << ' ';
    }
    cout << endl;
    for (int i = 0; i < 11; i++)
    {
        cout << b[i] << ' ';
    }
    puts("=== ===\n\n");
}


int main() {
    test_insertion();
    test_deletion();
    test_both();
    special_test();
    test_copy_constructor();
    hard_test(100000, 100000);
}
