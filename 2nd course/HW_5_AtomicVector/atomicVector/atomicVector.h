#ifndef ATOMICVECTOR_H_INCLUDED
#define ATOMICVECTOR_H_INCLUDED

#include <iostream>
#include <thread>
#include <mutex>

template <typename _T> class atomicVector
{
    typedef unsigned _size_t;
    typedef _T* _ptr_t;
public:
    // Pointers get
    _ptr_t begin()
    {
        return _begin;
    }
    _ptr_t end()
    {
        if (_begin == nullptr) {
            return nullptr;
        }
        return _begin + _size;
    }

    // Size get
    _size_t size ()
    {
        return _size;
    }
    _size_t capacity ()
    {
        return _capacity;
    }

    // Data locally get/set
    _T &operator[] (_size_t __pos)
    {
        if (_begin == nullptr) {
            throw "empty vector";
        }
        if (__pos >= _size && __pos < _capacity) {
            _size = __pos + 1; // Increasing size. If you access element through this, you guarantee all previous elements to be correct as part of array
        }
        return _begin[__pos];
    }
    _T &at (_size_t __pos)
    {
        if (_begin == nullptr) {
            throw "empty vector";
        }
        if (__pos >= _size) {
            throw "out of range";
        }
        return _begin[__pos];
    }

    void push_back (_T __data)
    {
        if (_capacity <= _size) {
            reserve(1);
        }
        _begin[_size] = __data;
        _size++;
    }
    void push_before (_T __data, _size_t __pos)
    {
        if (__pos >= _size) {
            throw "out of range";
        }
        if (_capacity <= _size) {
            reserve(1);
        }
        for (_size_t i = _size; i >= __pos; i--) {
            _begin[i + 1] = _begin[i];
        }
        _begin[__pos] = __data;
        _size++;
    }
    void push_front (_T __data)
    {
        push_before(__data, 0);
    }

    _T pop (_size_t __pos)
    {
        if (__pos >= _size) {
            throw "out of range";
        }
        _T result = _begin[__pos];
        for (_size_t i = __pos; i < _size; i++) {
            _begin[i] = _begin[i + 1];
        }
        _size--;
        return result;
    }
    _T pop_back ()
    {
        return pop(_size - 1);
    }
    _T pop_front ()
    {
        return pop(0);
    }

    // Data and capacity global changes
    void fill (_T __data)
    {
        for (_size_t i = 0; i < _capacity; i++) {
            _begin[i] = __data;
        }
        _size = _capacity;
    }
    void clear ()
    {
        _size = 0;
    }
    void flip ()
    {
        for (_size_t i = 0; i < (_size / 2); i++) {
            _T temp = _begin[i];
            _begin[i] = _begin[_size - i -1];
            _begin[_size - i - 1] = temp;
        }
    }

    // Memory
    void resize (_size_t __capacity)
    {
        if (__capacity == _capacity) {
            return;
        }
        atomicVector <_T> other(__capacity);
        for (_size_t i = 0; i < __capacity && i < _size; i++) {
            other[i] = _begin[i];
        }
        swap(*this, other);
    }
    void reserve (_size_t __size)
    {
        resize(_capacity + __size);
    }
    void shrink_to_fit ()
    {
        resize(_size);
    }

    // Constructor
    atomicVector ()
    {
        _size = 0;
        _capacity = 0;
        _begin = nullptr;
    }
    atomicVector (_size_t __capacity)
    {
        _size = 0;
        _capacity = __capacity;
        _begin = new _T[__capacity];
    }
    atomicVector (_size_t __capacity, _T __data)
    {
        _size = 0;
        _capacity = __capacity;
        _begin = new _T[__capacity];
        fill(__data);
    }

    // 5
    ~atomicVector ()
    {
        delete [] _begin;
    }
    atomicVector (const atomicVector <_T> &__other)
    {
        _size = __other._size;
        _capacity = __other._capacity;
        _begin = new _T[_capacity];
        for (_size_t i = 0; i < _size; i++) {
            _begin[i] = __other._begin[i];
        }
    }
    atomicVector (atomicVector <_T> &&__other)
    {
        delete [] _begin;
        swap(*this, __other);
    }
    atomicVector <_T> operator= (const atomicVector <_T> &__other)
    {
        atomicVector <_T> other(__other);
        swap(*this, other);
        return *this;
    }
    atomicVector <_T> operator= (atomicVector <_T> &&__other)
    {
        delete [] _begin;
        swap(*this, __other);
        return *this;
    }

    // iostream
    friend std::ostream &operator<< (std::ostream &__stream, atomicVector <_T> __vector)
    {
        for (_size_t i = 0; i < __vector._size; i++) {
            __stream << __vector._begin[i] << " ";
        }
        return __stream;
    }
private:
    _ptr_t _begin;
    _size_t _size;
    _size_t _capacity;

    std::mutex _mutex;

    friend void swap (atomicVector <_T> &first, atomicVector <_T> &second)
    {
        _size_t fSize = first._size;
        _size_t fCap = first._capacity;
        _ptr_t fBegin = first._begin;

        first._size = second._size;
        first._capacity = second._capacity;
        first._begin = second._begin;

        second._size = fSize;
        second._capacity = fCap;
        second._begin = fBegin;
    }
};

#endif // ATOMICVECTOR_H_INCLUDED
