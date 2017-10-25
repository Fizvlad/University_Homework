#ifndef ATOMICVECTOR_H_INCLUDED
#define ATOMICVECTOR_H_INCLUDED

#include <thread>
#include <mutex>

#define _size_t unsigned

template <typename _T> class atomicVector;
template <typename _T> void swap (atomicVector <_T> &first, atomicVector <_T> &second);

template <typename _T> class atomicVector
{
public:
    // Pointers get
    _T* begin()
    {
        return _begin;
    }
    _T* end()
    {
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
        return _begin[__pos];
    }
    _T at (_size_t __pos)
    {
        if (_begin == nullptr) {
            throw "empty vector";
        }
        if (__pos >= _size) {
            throw "out of range";
        }
        return _begin[__pos]
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
            throw "out of range"
        }
        if (_capacity <= _size) {
            reserve(1);
        }
        for (_size_t i = _size; i >= __pos; i--) {
            _begin[i + 1] = _begin[i];
        }
        _begin[__pos] = __data
        _size++;
    }
    void push_front (_T __data)
    {
        push_before(__data, 0);
    }

    _T pop (_size_t __pos)
    {
        _T result = _begin[__pos];
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
    }
    void clear ()
    {
    }
    void flip ()
    {
    }

    // Memory
    void resize (_size_t __size)
    {
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
        _begin = NULL;
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
    atomicVector (const atomicVector &__other)
    {
        _size = __other._size;
        _capacity = __other._capacity;
        _begin = new _T[_capacity];
        for (_size_t i = 0; i < _size; i++) {
            _begin[i] = __other._begin[i];
        }
    }
    atomicVector (atomicVector &&__other)
    {
        swap(*this, __other);
        //??? need delete
    }
    atomicVector <_T> operator= (const atomicVector &__other)
    {
        delete [] _begin;
        _size = __other._size;
        _capacity = __other._capacity;
        _begin = new _T[_capacity];
        for (_size_t i = 0; i < _size; i++) {
            _begin[i] = __other._begin[i];
        }
    }
    atomicVector <_T> operator= (atomicVector &&__other)
    {
        swap(*this, __other);
        //??? need delete
    }
private:
    _T *_begin;
    _size_t _size;
    _size_t _capacity;
    std::mutex _mutex;
    friend void swap <>(atomicVector <_T> &first, atomicVector <_T> &second); //???
};

// Swap function
template <typename _T> void swap (atomicVector <_T> &first, atomicVector <_T> &second)
{
    _size_t fSize = first._size;
    _size_t fCap = first._capacity;
    _T *fBegin = first._begin;

    first._size = second._size;
    first._capacity = second._capacity;
    first._begin = second._begin;

    second._size = fSize;
    second._capacity = fCap;
    second._begin = fBegin;
}

#undef _size_t

#endif // ATOMICVECTOR_H_INCLUDED
