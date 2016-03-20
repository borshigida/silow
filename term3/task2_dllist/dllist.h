#ifndef DLL_H
#define DLL_H
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <cmath>
#include <cassert>

using namespace std;

template<typename _Tp>
struct _dllist_node {
    _Tp _M_data;
    _dllist_node* _M_next;
    _dllist_node* _M_prev;
    
    void _M_hook(_dllist_node* const _position)
    {
        this->_M_next = _position;
        this->_M_prev = _position->_M_prev;
        
        _position->_M_prev->_M_next = this;
        _position->_M_prev = this;
    }
    
    void _M_unhook()
    {
        this->_M_prev->_M_next = this->_M_next;
        this->_M_next->_M_prev = this->_M_prev;
    }
};

template<typename _Tp>
struct _dllist_iterator
{
    typedef _dllist_iterator<_Tp> _Self;
    typedef _dllist_node<_Tp> _Node;
    typedef _Tp value_type;
    typedef _Tp* pointer;
    typedef _Tp& reference;
    
    _Node* _M_node; // pointer!
    
    _dllist_iterator()
    : _M_node() { }
    
    _dllist_iterator(_Node* _x)
    : _M_node(_x) { }
    
    reference operator*() const
    { return _M_node->_M_data; }
    
    _Self& operator++()
    {
        _M_node = _M_node->_M_next;
        return *this;
    }
    
    _Self& operator--()
    {
        _M_node = _M_node->_M_prev;
        return *this;
    }
    
    bool operator==(const _Self& _x) const
    { return _M_node == _x._M_node; }
    
    bool operator!=(const _Self& _x) const
    { return _M_node != _x._M_node; }
};

template<typename _Tp>
struct _dllist_const_iterator {
    typedef _dllist_const_iterator<_Tp> _Self;
    typedef const _dllist_node<_Tp> _Node;
    typedef _Tp value_type;
    typedef const _Tp* pointer;
    typedef const _Tp& reference;
    
    const _Node* _M_node; // pointer!
    
    _dllist_const_iterator()
    : _M_node() { }
    
    _dllist_const_iterator(const _Node* _x)
    : _M_node(_x) { }
    
    reference operator*() const
    { return _M_node->_M_data; }
    
    _Self& operator++()
    {
        _M_node = _M_node->_M_next;
        return *this;
    }
    
    _Self& operator--()
    {
        _M_node = _M_node->_M_prev;
        return *this;
    }
    
    bool operator==(const _Self& _x) const
    { return _M_node == _x._M_node; }
    
    bool operator!=(const _Self& _x) const
    { return _M_node != _x._M_node; }
};

template<typename _Tp>
class dllist {
    _dllist_node<_Tp> _tail;
    
public:
    typedef _dllist_iterator<_Tp> iterator;
    typedef _dllist_const_iterator<_Tp> const_iterator;
    typedef _dllist_node<_Tp> _Node;
    
    
    // === CONSTRUCTORS AND DESTRUCTORS ===
    
    
    dllist();
    // Default constructor creates no elements.
    
    dllist(size_t _n);
    // Creates a list with default constructed elements.
    
    dllist(size_t _n, const _Tp& _x);
    // Creates a list with copies of an exemplar element.
    
    dllist(const dllist& _x);
    // List copy constructor.
    
    ~dllist();
    // Destroys the list.
    
    
    // === ===
    
    
    dllist& operator=(const dllist& a); // copy assignment
    
    
    iterator begin()
    { return iterator(this->_tail._M_next); }
    
    const_iterator begin() const
    { return const_iterator(this->_tail._M_next); }
    
    iterator end()
    { return iterator(&this->_tail); }
    
    const_iterator end() const
    { return const_iterator(&this->_tail); }
    
    
    iterator insert(iterator _position, const _Tp& _x);
    iterator erase(iterator _position);
    iterator erase(iterator _first, iterator _last);
    
    size_t size(); 

private:
    
    _Node* _M_create_node(const _Tp& _x);
    _Node* _M_create_node();
    
    void _M_insert(iterator _position, const _Tp& _x);
    void _M_insert(iterator _position);
    void _M_erase(iterator _position);
    
    void _M_default_initialize(size_t _n);
    void _M_fill_initialize(size_t _n, const _Tp& _x);
    void _M_initialize_dispatch(const_iterator _first, const_iterator _last);
    
    friend ostream& operator <<(ostream& out, const dllist& _x)
    {
        for(const_iterator a = _x.begin(); a != _x.end(); ++a)
            out << a._M_node->_M_data << " " << (a._M_node->_M_next == _x.end()._M_node ? "" : "<-> ");
        out << endl;
        return out;
    }
};





template<typename _Tp>
dllist<_Tp>::dllist()
// Default constructor creates no elements.
: _tail()
{
    _tail._M_next = &_tail;
    _tail._M_prev = &_tail;
}

template<typename _Tp>
dllist<_Tp>::dllist(size_t _n)
// Creates a list with default constructed elements.
: _tail()
{
    _tail._M_next = &_tail;
    _tail._M_prev = &_tail;
    
    this->_M_default_initialize(_n);
}

template<typename _Tp>
dllist<_Tp>::dllist(size_t _n, const _Tp& _x)
// Creates a list with copies of an exemplar element.
: _tail()
{
    _tail._M_next = &_tail;
    _tail._M_prev = &_tail;
    
    this->_M_fill_initialize(_n, _x);
}

template<typename _Tp>
dllist<_Tp>::dllist(const dllist& _x)
// List copy constructor.
: _tail()
{
    _tail._M_next = &_tail;
    _tail._M_prev = &_tail;
    
    this->_M_initialize_dispatch(_x.begin(), _x.end());
}

template<typename _Tp>
dllist<_Tp>::~dllist()
// Destroys the list.
{
    _dllist_node<_Tp>* _cur = _tail._M_next;
    while (_cur != &this->_tail)
    {
        _dllist_node<_Tp>* _tmp = _cur;
        _cur = _cur->_M_next;
        delete _tmp;
    }
}

template<typename _Tp>
dllist<_Tp>& dllist<_Tp>::operator=(const dllist& _x)
{
    if (this != &_x)
    {
        iterator _first1 = this->begin();
        iterator _last1 = this->end();
        const_iterator _first2 = _x.begin();
        const_iterator _last2 = _x.end();
        for(; _first1 != _last1 && _first2 != _last2; ++_first1, ++_first2)
        {
            *_first1 = *_first2;
        }
        if (_first2 == _last2)
        {
            erase(_first1, _last1);
        }
        else
        {
            for(; _first2 != _last2; ++_first1, ++_first2)
            {
                this->insert(this->end(), *_first2);
            }
            // insert(_last1, _first2, _last2);
        }
    }
    return *this;
}

template<typename _Tp>
size_t dllist<_Tp>::size()
{
    int _i = 0;
    iterator _it = this->begin();
    for (; _it != this->end(); ++_i, ++_it);
    return _i;
}


template<typename _Tp>
_dllist_node<_Tp>* dllist<_Tp>::_M_create_node(const _Tp& _x)
{
    _Node* _p = new _Node;
    try
    {
        _p->_M_data = _Tp(_x);
    }
    catch(...)
    {
        delete _p;
        throw(-1);
    }
    return _p;
}
template<typename _Tp>
_dllist_node<_Tp>* dllist<_Tp>::_M_create_node()
{
    _Node* _p = new _Node;
    try
    {
        _p->_M_data = _Tp(); // ??
    }
    catch(...)
    {
        delete _p;
        throw(-1);
    }
    return _p;
}

template<typename _Tp>
void dllist<_Tp>::_M_insert(iterator _position, const _Tp& _x)
{
    _Node* _tmp = _M_create_node(_x);
    _tmp->_M_hook(_position._M_node);
}
template<typename _Tp>
void dllist<_Tp>::_M_insert(iterator _position)
{
    _Node* _tmp = _M_create_node();
    _tmp->_M_hook(_position._M_node);
}

template<typename _Tp>
void dllist<_Tp>::_M_erase(iterator _position)
{
    try
    {
        if (_position==end())
            throw(-1);
        _position._M_node->_M_unhook();
        delete _position._M_node;
    }
    catch(...)
    {
        cerr << "Couldn't erase" << endl;
        exit(-1);
    }
}

template<typename _Tp>
void dllist<_Tp>::_M_default_initialize(size_t _n)
{
    for(; _n; --_n)
    {
        this->_M_insert(end());
    }
}

template<typename _Tp>
void dllist<_Tp>::_M_fill_initialize(size_t _n, const _Tp& _x)
{
    for(; _n; --_n)
    {
        this->_M_insert(end(), _x);
    }
}

template<typename _Tp>
void dllist<_Tp>::_M_initialize_dispatch(const_iterator _first, const_iterator _last)
{
    for(; _first != _last; ++_first)
    {
        this->_M_insert(end(), *_first);
    }
}

template<typename _Tp>
typename dllist<_Tp>::iterator dllist<_Tp>::insert(iterator _position, const _Tp& _x)
{
    _Node* _tmp = _M_create_node(_x);
    _tmp->_M_hook(_position._M_node);
    return iterator(_tmp);
}

template<typename _Tp>
typename dllist<_Tp>::iterator dllist<_Tp>::erase(iterator _position)
{
    iterator _ret = iterator(_position._M_node->_M_next);
    _M_erase(_position);
    return _ret;
}
template<typename _Tp>
typename dllist<_Tp>::iterator dllist<_Tp>::erase(iterator _first, iterator _last)
{
    while(_first != _last)
        _first = this->erase(_first);
    return _last;
}



#endif // DLL_H
