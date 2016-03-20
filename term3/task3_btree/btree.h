#ifndef _BTREE_H_
#define _BTREE_H_
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <cmath>
#include <cassert>

using namespace std;

template<typename _T1, typename _T2>
struct mpair
{
    _T1 first;
    _T2 second;
    
    mpair() : first(), second() { }
    mpair(const _T1& a, const _T2& b) : first(a), second(b) { }
};

template<typename key_type, typename val_type, int t>
struct _btree_node
{
    int n;
    mpair<key_type, val_type> *keys;
    bool leaf;
    _btree_node<key_type, val_type, t>* *pointers;
    
public:
    _btree_node();
    ~_btree_node();
};

template<typename key_type, typename val_type, int t>
_btree_node<key_type, val_type, t>::_btree_node()
{
    //cerr << "_btree_node constructor" << endl;
    this->n = 0;
    this->leaf = false;
    this->keys = new mpair<key_type, val_type>[2*t + 1];
    this->pointers = new _btree_node<key_type, val_type, t>*[2*t];
}

template<typename key_type, typename val_type, int t>
_btree_node<key_type, val_type, t>::~_btree_node()
{
    //cerr << "_btree_node destructor" << endl;
    delete[] this->keys;
    delete[] this->pointers;
}

template<typename key_type, typename val_type, int t>
class btree
{
protected:
    _btree_node<key_type, val_type, t>* _root;
    
public:
    btree();
    btree(const btree<key_type, val_type, t>& x);
    btree<key_type, val_type, t>& operator=(const btree<key_type, val_type, t>& x);
    ~btree();
    
    val_type& operator[](const key_type& k);
    void del(const key_type& k);
    int len();
    
private:
    val_type& search(_btree_node<key_type, val_type, t>& x, const key_type& k);  // check it!
    void split_child(_btree_node<key_type, val_type, t>& x, int i);
    val_type& insert(const mpair<key_type, val_type>& k);
    val_type& insert_nonfull(_btree_node<key_type, val_type, t>& x, const mpair<key_type, val_type>& k);
    void _delete(_btree_node<key_type, val_type, t>& x, const key_type& k);
    void _freeall(_btree_node<key_type, val_type, t>& x);
    int _len(_btree_node<key_type, val_type, t>& x);
    void _copy(_btree_node<key_type, val_type, t>& x, const _btree_node<key_type, val_type, t>& y);
};

// === CREATING ===

template<typename key_type, typename val_type, int t>
btree<key_type, val_type, t>::btree()
{
    this->_root = new _btree_node<key_type, val_type, t>;
    // cerr << "allocating " << &this->_root << endl;
    _btree_node<key_type, val_type, t>& _x = *this->_root;
    _x.leaf = true;
    _x.n = 0;
}

template<typename key_type, typename val_type, int t>
btree<key_type, val_type, t>::btree(const btree<key_type, val_type, t>& x)
{
    this->_root = new _btree_node<key_type, val_type, t>;
    _copy(*this->_root, *x._root);
}

template<typename key_type, typename val_type, int t>
void btree<key_type, val_type, t>::_copy(_btree_node<key_type, val_type, t>& x, const _btree_node<key_type, val_type, t>& y)
{
    x.n = y.n;
    x.leaf = y.leaf;
    for (int i = 0; i < x.n; i++)
    {
        x.keys[i] = y.keys[i];
    }
    if (!x.leaf)
    {
        for (int i = 0; i < x.n + 1; i++)
        {
            x.pointers[i] = new _btree_node<key_type, val_type, t>;
            _copy(*x.pointers[i], *y.pointers[i]);
        }
    }
}

template<typename key_type, typename val_type, int t>
btree<key_type, val_type, t>& btree<key_type, val_type, t>::operator=(const btree<key_type, val_type, t>& x)
{
    if (this == &x)
        return *this;
    
    if (!this->_root->leaf)
    {
        for (int i = 0; i < this->_root->n + 1; i++)
            _freeall(*(this->_root->pointers[i]));
    }
    
    delete this->_root;
    this->_root = new _btree_node<key_type, val_type, t>;
    _copy(*this->_root, *x._root);
    
    return *this;
}
// ===   ===

template<typename key_type, typename val_type, int t>
void btree<key_type, val_type, t>::_freeall(_btree_node<key_type, val_type, t>& x)
{
    if (x.leaf)
        delete &x;
    else
    {
        for (int i = 0; i < x.n + 1; i++)
            _freeall(*x.pointers[i]);
        delete &x;
    }
}

template<typename key_type, typename val_type, int t>
btree<key_type, val_type, t>::~btree()
{
    if (!this->_root->leaf)
    {
        for (int i = 0; i < this->_root->n + 1; i++)
            _freeall(*(this->_root->pointers[i]));
    }
    
    delete this->_root;
}

// === SEARCHING ===
template<typename key_type, typename val_type, int t>
val_type& btree<key_type, val_type, t>::search(_btree_node<key_type, val_type, t>& x, const key_type& k)
{
    int i = 0;
    while (i < x.n && k > x.keys[i].first)
        i++;
    if (i < x.n && k == x.keys[i].first)
        return x.keys[i].second;
    if (x.leaf)
        return this->insert(mpair<key_type, val_type>(k, val_type()));
    return this->search(*x.pointers[i], k);
}

template<typename key_type, typename val_type, int t>
val_type& btree<key_type, val_type, t>::operator[](const key_type& k)
{
    return this->search(*this->_root, k);
}
// ===   ====


// === INSERTION ===
template<typename key_type, typename val_type, int t>
void btree<key_type, val_type, t>::split_child(_btree_node<key_type, val_type, t>& x, int i)
{
    _btree_node<key_type, val_type, t>& z = *new _btree_node<key_type, val_type, t>;
    //cerr << "allocating " << &z << endl;
    _btree_node<key_type, val_type, t>& y = *x.pointers[i];
    
    z.leaf = y.leaf;
    z.n = t - 1;
    for (int j = 0; j < t - 1; j++)
        z.keys[j] = y.keys[j + t];
    if (!y.leaf)
    {
        for (int j = 0; j < t; j++)
            z.pointers[j] = y.pointers[j + t];
    }
    y.n = t - 1;
    for (int j = x.n; j >= i + 1; j--)
        x.pointers[j + 1] = x.pointers[j];
    x.pointers[i + 1] = &z;
    for (int j = x.n - 1; j >= i; j--)
        x.keys[j + 1] = x.keys[j];
    x.keys[i] = y.keys[t-1];
    x.n = x.n + 1;
}

template<typename key_type, typename val_type, int t>
val_type& btree<key_type, val_type, t>::insert(const mpair<key_type, val_type>& k)
{
    _btree_node<key_type, val_type, t>* r = this->_root;
    if (r->n == 2*t-1)
    {
        this->_root = new _btree_node<key_type, val_type, t>;
        _btree_node<key_type, val_type, t>& s = *this->_root;
        s.leaf = false;
        s.n = 0;
        s.pointers[0] = r;
        this->split_child(s, 0);
        return this->insert_nonfull(s, k);
    }
    else
    {
        return insert_nonfull(*this->_root, k);
    }
}

template<typename key_type, typename val_type, int t>
val_type& btree<key_type, val_type, t>::insert_nonfull(_btree_node<key_type, val_type, t>& x, const mpair<key_type, val_type>& k)
{
    int i = x.n - 1;
    if (x.leaf)
    {
        while (i >= 0 && k.first < x.keys[i].first)
        {
            x.keys[i+1] = x.keys[i];
            i--;
        }
        x.keys[i+1] = k;
        x.n = x.n + 1;
        return x.keys[i+1].second;
    }
    else
    {
        while (i >= 0 && k.first < x.keys[i].first)
        {
            i--;
        }
        i++;
        if (x.pointers[i]->n == 2*t-1)
        {
            this->split_child(x, i);
            if (k.first > x.keys[i].first)
            {
                i++;
            }
        }
        return this->insert_nonfull(*x.pointers[i], k);
    }
}
// ===   ====


// === DELETION ===

template<typename key_type, typename val_type, int t>
void btree<key_type, val_type, t>::_delete(_btree_node<key_type, val_type, t>& x, const key_type& k)
{
    // 1
    if (x.leaf)
    {
        int i = 0;
        while (i < x.n && k > x.keys[i].first)
            i++;
        if (i < x.n && k == x.keys[i].first)
        {
            for (int j = i; j < x.n - 1; j++)
            {
                x.keys[j] = x.keys[j + 1];
            }
            x.n--;
        }
        else
        {
            throw -1;
        }
        return;
    }
    
    // 2
    int i = 0;
    while (i < x.n && k > x.keys[i].first)
        i++;
    if (i < x.n && k == x.keys[i].first)
    {
        // 2a
        _btree_node<key_type, val_type, t> &y = *x.pointers[i], &z = *x.pointers[i + 1];
        if (y.n >= t)
        {
            mpair<key_type, val_type> k_one = y.keys[y.n - 1];
            _delete(y, k_one.first);
            x.keys[i] = k_one;
        }
        // 2b
        else if (z.n >= t)
        {
            mpair<key_type, val_type> k_one = z.keys[0];
            _delete(z, k_one.first);
            x.keys[i] = k_one;
        }
        // 2c
        else
        {
            y.keys[t-1] = x.keys[i];
            for (int j = 0; j < t-1; ++j)
                y.keys[t + j] = z.keys[j];
            for (int j = 0; j < t; ++j)
                y.pointers[t + j] = z.pointers[j];
            
            y.n = 2*t - 1;
            for (int j = i; j < x.n - 1; ++j)
                x.keys[j] = x.keys[j + 1];
            for (int j = i + 1; j < x.n; ++j)
                x.pointers[j] = x.pointers[j + 1];
            x.n--;
            
            delete (&z);
            
            _delete(y, k);
        }
    }
    else
    {
        // 3
        _btree_node<key_type, val_type, t> &y = *x.pointers[i];
        if (y.n >= t)
        {
            try
            {
                _delete(y, k);
            }
            catch(...)
            {
                throw(-1);
            }
            return;
        }
        
        // 3a
        if (i + 1 < x.n + 1 && x.pointers[i+1]->n >= t)
        {
            _btree_node<key_type, val_type, t> &z = *x.pointers[i+1];
            y.keys[t-1] = x.keys[i];
            x.keys[i] = z.keys[0];
            y.pointers[t] = z.pointers[0];
            y.n++;
            for (int j = 0; j < z.n-1; j++)
                z.keys[j] = z.keys[j + 1];
            for (int j = 0; j < z.n; j++)
                z.pointers[j] = z.pointers[j + 1];
            z.n--;
            
            try
            {
                _delete(y, k);
            }
            catch(...)
            {
                throw(-1);
            }
            return;
        }
        else if (i - 1 > -1 && x.pointers[i-1]->n >= t)
        {
            _btree_node<key_type, val_type, t> &z = *x.pointers[i-1];
            
            for (int j = t-1; j > 0; j--)
                y.keys[j] = y.keys[j-1];
            for (int j = t; j > 0; j--)
                y.pointers[j] = y.pointers[j-1];
            y.keys[0] = x.keys[i-1];
            x.keys[i-1] = z.keys[z.n-1];
            y.pointers[0] = z.pointers[z.n];
            y.n++;
            z.n--;
            
            try
            {
                _delete(y, k);
            }
            catch(...)
            {
                throw(-1);
            }
            return;
        }
        // 3b
        else if (i + 1 < x.n + 1)
        {
            _btree_node<key_type, val_type, t> &z = *x.pointers[i+1];
            
            y.keys[t-1] = x.keys[i];
            for (int j = 0; j < t-1; ++j)
                y.keys[t + j] = z.keys[j];
            for (int j = 0; j < t; ++j)
                y.pointers[t + j] = z.pointers[j];
            
            y.n = 2*t - 1;
            for (int j = i; j < x.n - 1; ++j)
                x.keys[j] = x.keys[j + 1];
            for (int j = i + 1; j < x.n; ++j)
                x.pointers[j] = x.pointers[j + 1];
            x.n--;
            
            delete (&z);
            
            try
            {
                _delete(y, k);
            }
            catch(...)
            {
                throw(-1);
            }
            return;
        }
        else if (i - 1 > -1)
        {
            _btree_node<key_type, val_type, t> &z = *x.pointers[i-1];
            
            z.keys[t-1] = x.keys[i-1];
            for (int j = 0; j < t-1; ++j)
                z.keys[t + j] = y.keys[j];
            for (int j = 0; j < t; ++j)
                z.pointers[t + j] = y.pointers[j];
            
            z.n = 2*t - 1;
            for (int j = i-1; j < x.n - 1; ++j)
                x.keys[j] = x.keys[j + 1];
            for (int j = i; j < x.n; ++j)
                x.pointers[j] = x.pointers[j + 1];
            x.n--;
            
            delete (&y);
            
            try
            {
                _delete(z, k);
            }
            catch(...)
            {
                throw(-1);
            }
            return;
        }
    }
}

template<typename key_type, typename val_type, int t>
void btree<key_type, val_type, t>::del(const key_type& k)
{
    try
    {
        this->_delete(*this->_root, k);
    }
    catch(...)
    {
        cerr << "Couldn't delete\n";
        exit(-1);
    }
}

// ===   ===

template<typename key_type, typename val_type, int t>
int btree<key_type, val_type, t>::_len(_btree_node<key_type, val_type, t>& x)
{
    int res = x.n;
    if (!x.leaf)
        for (int i = 0; i < x.n + 1; ++i)
            res += _len(*x.pointers[i]);
    return res;
}

template<typename key_type, typename val_type, int t>
inline int btree<key_type, val_type, t>::len()
{
    return _len(*this->_root);
}

#endif /* _BTREE_H_ */
