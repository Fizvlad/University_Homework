#ifndef THREADVECTOR_H_INCLUDED
#define THREADVECTOR_H_INCLUDED

#include <thread>
#include <mutex>

#define _size_t unsigned

template <typename _T> class threadVector
{
public:
    // Pointers get
    _T* begin()
    {
        return _begin;
    }
    _T* end()
    {
        return _begin + _size
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
        return _begin[__pos];
    }
    _T at (_size_t __pos)
    {
    }

    void push_back (_T __data)
    {
    }
    void push_before (_T __data, _size_t __pos)
    {
    }
    void push_front (_T __data)
    {
        push_before(__data, 0);
    }

    _T pop (_size_t __pos)
    {
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
    }

    // Constructor
    threadVector ()
    {
    }
    threadVector (_size_t __capacity)
    {
    }
    threadVector (_size_t __capacity, _T __data)
    {
    }

    // 5
    ~threadVector ()
    {
    }
    threadVector (const threadVector &__other)
    {
    }
    threadVector (threadVector &&__other)
    {
    }
    threadVector <_T> operator= (const threadVector &__other)
    {
    }
    threadVector <_T> operator= (threadVector &&__other)
    {
    }
private:
    _T *_begin;
    _size_t _size;
    _size_t _capacity;
    std::mutex _mutex;
};

// Swap function
template <class _T> void swap (threadVector <_T> &first, threadVector <_T> &second)
{

}

#undef _size_t

#endif // THREADVECTOR_H_INCLUDED
