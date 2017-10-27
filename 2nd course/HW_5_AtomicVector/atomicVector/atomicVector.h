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
        _mutex.lock();
        if (_begin == nullptr) {
            throw err_null_vector();
        }
        if (__pos >= _size && __pos < _capacity) {
            _size = __pos + 1; // Increasing size. If you access element through this, you guarantee all previous elements to be correct as part of array
        }
        _mutex.unlock();
        return _begin[__pos];
    }
    _T &at (_size_t __pos)
    {
        _mutex.lock();
        if (_begin == nullptr) {
            throw err_null_vector();
        }
        if (__pos >= _size) {
            throw err_out_of_range();
        }
        _mutex.unlock();
        return _begin[__pos];
    }

    void push_back (_T __data)
    {
        _mutex.lock();
        if (_capacity <= _size) {
            reserve(1);
        }
        _begin[_size] = __data;
        _size++;
        _mutex.unlock();
    }
    void push_before (_T __data, _size_t __pos)
    {
        _mutex.lock();
        if (__pos >= _size) {
            throw err_out_of_range();
        }
        if (_capacity <= _size) {
            reserve(1);
        }
        for (_size_t i = _size; i > __pos; i--) {
            _begin[i] = _begin[i - 1];
        }
        _begin[__pos] = __data;
        _size++;
        _mutex.unlock();
    }
    void push_front (_T __data)
    {
        push_before(__data, 0);
    }

    _T pop (_size_t __pos)
    {
        _mutex.lock();
        if (__pos >= _size) {
            throw err_out_of_range();
        }
        _T result = _begin[__pos];
        for (_size_t i = __pos; i < _size; i++) {
            _begin[i] = _begin[i + 1];
        }
        _size--;
        _mutex.unlock();
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
        _mutex.lock();
        for (_size_t i = 0; i < _capacity; i++) {
            _begin[i] = __data;
        }
        _size = _capacity;
        _mutex.unlock();
    }
    void clear ()
    {
        _mutex.lock();
        _size = 0;
        _mutex.unlock();
    }
    void flip ()
    {
        _mutex.lock();
        for (_size_t i = 0; i < (_size / 2); i++) {
            _T temp = _begin[i];
            _begin[i] = _begin[_size - i -1];
            _begin[_size - i - 1] = temp;
        }
        _mutex.unlock();
    }

    // Memory
    void resize (_size_t __capacity)
    {
        _mutex.lock();
        if (__capacity == _capacity) {
            return;
        }
        atomicVector <_T> other(__capacity);
        for (_size_t i = 0; i < __capacity && i < _size; i++) {
            other[i] = _begin[i];
        }
        swap(*this, other);
        _mutex.unlock();
    }
    void reserve (_size_t __size)
    {
        _mutex.lock();
        resize(_capacity + __size);
        _mutex.unlock();
    }
    void shrink_to_fit ()
    {
        _mutex.lock();
        resize(_size);
        _mutex.unlock();
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
        _mutex.lock();
        _size = __other._size;
        _capacity = __other._capacity;
        _begin = new _T[_capacity];
        for (_size_t i = 0; i < _size; i++) {
            _begin[i] = __other._begin[i];
        }
        _mutex.unlock();
    }
    atomicVector (atomicVector <_T> &&__other)
    {
        _mutex.lock();
        delete [] _begin;
        swap(*this, __other);
        _mutex.unlock();
    }
    atomicVector <_T> operator= (const atomicVector <_T> &__other)
    {
        _mutex.lock();
        atomicVector <_T> other(__other);
        swap(*this, other);
        _mutex.unlock();
        return *this;
    }
    atomicVector <_T> operator= (atomicVector <_T> &&__other)
    {
        _mutex.lock();
        delete [] _begin;
        swap(*this, __other);
        _mutex.unlock();
        return *this;
    }

    // <iostream>
    friend std::ostream &operator<< (std::ostream &__stream, atomicVector <_T> __vector)
    {
        __vector._mutex.lock();
        for (_size_t i = 0; i < __vector._size; i++) {
            __stream << __vector._begin[i] << " ";
        }
        __vector._mutex.unlock();
        return __stream;
    }

    // Errors
    class err_null_vector : std::exception
    {
        const char* what ()
        {
            return "Requested vector was not initialized yet";
        }
    };
    class err_out_of_range : std::exception
    {
        const char* what ()
        {
            return "Requested element is out of range";
        }
    };

    // Swap
    friend void swap (atomicVector <_T> &first, atomicVector <_T> &second)
    {
        __first._mutex.lock();
        _size_t fSize = first._size;
        _size_t fCap = first._capacity;
        _ptr_t fBegin = first._begin;

        first._size = second._size;
        first._capacity = second._capacity;
        first._begin = second._begin;

        second._size = fSize;
        second._capacity = fCap;
        second._begin = fBegin;
        __first._mutex.unlock(); // !!!
    }
private:
    _ptr_t _begin;
    _size_t _size;
    _size_t _capacity;

    std::mutex _mutex;
};

#endif // ATOMICVECTOR_H_INCLUDED
