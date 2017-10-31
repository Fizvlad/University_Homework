#ifndef FIGURES_H_INCLUDED
#define FIGURES_H_INCLUDED

#include <cmath>
#include <iostream>

typedef double coord_t;

class point;
class direct;
class polygon;

struct err_Wrong_direct : std::exception
{
    const char* what()
    {
        return "Wrong initialization of direct";
    }
};
struct err_Parallel_directions : std::exception
{
    const char* what()
    {
        return "Parallel or equal directions given";
    }
};

class point // (x, y)
{
    coord_t _x;
    coord_t _y;

    // Setters
    coord_t x (coord_t __x)
    {
        _x = __x;
        return _x;
    }
    coord_t y (coord_t __y)
    {
        _y = __y;
        return _y;
    }
    void set (coord_t __x, coord_t __y)
    {
        _x = __x;
        _y = __y;
    }
    // Operator=
    point &operator= (const point &__point)
    {
        set(__point.x(), __point.y());
        return *this;
    }
    point &operator= (point &&__point)
    {
        set(__point.x(), __point.y());
        return *this;
    }
public:
    // Constructor
    point (coord_t __x = 0, coord_t __y = 0)
    {
        set(__x, __y);
    }
    // 5. operator= locked. Copying allowed. Standart destructor
    point (const point &__point)
    {
        set(__point.x(), __point.y());
    }
    point (point &&__point)
    {
        set(__point.x(), __point.y());
    }

    // Getters
    coord_t x () const
    {
        return _x;
    }
    coord_t y () const
    {
        return _y;
    }
    // Other
    coord_t distance (point &__point) const
    {
        return std::sqrt(std::pow(x() - __point.x(), 2) + std::pow(y() - __point.y(), 2));
    }
};

class direct // ax + by + c = 0
{
    coord_t _a;
    coord_t _b;
    coord_t _c;

    // Safety
    void _check ()
    {
        if (_a == 0 && _b == 0) {
            throw err_Wrong_direct();
        }
    }

    // Setters
    coord_t a (coord_t __a)
    {
        _a = __a;
        _check();
        return _a;
    }
    coord_t b (coord_t __b)
    {
        _b = __b;
        _check();
        return _b;
    }
    coord_t c (coord_t __c)
    {
        _c = __c;
        _check();
        return _c;
    }
    void set (coord_t __a, coord_t __b, coord_t __c)
    {
        _a = __a;
        _b = __b;
        _c = __c;
        _check();
    }
    // Operator=
    direct &operator= (const direct &__direct)
    {
        set(__direct.a(), __direct.b(), __direct.c());
        return *this;
    }
    direct &operator= (direct &&__direct)
    {
        set(__direct.a(), __direct.b(), __direct.c());
        return *this;
    }
public:
    // Constructor
    direct (coord_t __a = 1, coord_t __b = -1, coord_t __c = 0)
    {
        set(__a, __b, __c);
    }
    // 5. operator= locked. Copying allowed. Standart destructor
    direct (const direct &__direct)
    {
        set(__direct.a(), __direct.b(), __direct.c());
    }
    direct (direct &&__direct)
    {
        set(__direct.a(), __direct.b(), __direct.c());
    }
    // Getters
    coord_t a () const
    {
        return _a;
    }
    coord_t b () const
    {
        return _b;
    }
    coord_t c () const
    {
        return _c;
    }
    // Other
    bool ifParallel (const direct &__direct) const
    {
        if (a() == 0 && __direct.a() == 0) {
            return true;
        }
        if (a() * __direct.a() != 0) {
            return false;
        }
        return (b() / a()) == (__direct.b() / __direct.a());
    }
    bool ifEqual (const direct &__direct) const
    {
        if (!ifParallel(__direct)) {
            return false;
        }
        if (a() == 0) {
            return (c() / b()) == (__direct.c() / __direct.b());
        }
        if (b() == 0) {
            return (c() / a()) == (__direct.c() / __direct.a());
        }
        return false;
    }

    point cross (const direct &__direct) const
    {
        if (ifParallel(__direct)) {
            throw err_Parallel_directions();
        }
        return point(0, 0);
    }
};

#endif // FIGURES_H_INCLUDED
