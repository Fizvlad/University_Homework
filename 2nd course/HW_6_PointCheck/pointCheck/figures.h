#ifndef FIGURES_H_INCLUDED
#define FIGURES_H_INCLUDED

#include <cmath>
#include <iostream>

typedef double coord_t;

// Definition
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
    coord_t x (const coord_t &__x)
    {
        _x = __x;
        return _x;
    }
    coord_t y (const coord_t &__y)
    {
        _y = __y;
        return _y;
    }
    void set (const coord_t &__x, const coord_t &__y)
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
    point (coord_t __x, coord_t __y)
    {
        set(__x, __y);
    }

    // 5. operator= locked. Copying allowed. Default destructor
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
    coord_t distance (const coord_t &__x, const coord_t &__y) const
    {
        return std::sqrt(std::pow(x() - __x, 2) + std::pow(y() - __y, 2));
    }
    coord_t distance (const point &__point) const
    {
        return distance(__point.x(), __point.y());
    }
};

class direct // ax + by + c = 0
{
    coord_t _a;
    coord_t _b;
    coord_t _c;

    // Safety
    void _check () const
    {
        if (_a == 0 && _b == 0) {
            throw err_Wrong_direct();
        }
    }

    // Setters
    coord_t a (const coord_t &__a)
    {
        _a = __a;
        _check();
        return _a;
    }
    coord_t b (const coord_t &__b)
    {
        _b = __b;
        _check();
        return _b;
    }
    coord_t c (const coord_t &__c)
    {
        _c = __c;
        _check();
        return _c;
    }
    void set (const coord_t &__a, const coord_t &__b, const coord_t &__c)
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
    direct (const coord_t __a, const coord_t __b, const coord_t __c)
    {
        set(__a, __b, __c);
    }
    direct (const point __point1, const point __point2)
    {
        if (__point1.x() == __point2.x()) {
            set(1, 0, (-1) * __point1.x());
        } else {
            coord_t a, b, c;
            a = (__point1.y() - __point2.y()) / (__point1.x() - __point2.x());
            b = 1;
            c = (-1) * (a * __point1.x() + b * __point1.y());
            set(a , b, c);
        }
    }

    // 5. operator= locked. Copying allowed. Default destructor
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
        if (a() * __direct.a() == 0) {
            return false;
        }
        return (b() / a()) == (__direct.b() / __direct.a());
    }
    bool ifEqual (const direct &__direct) const
    {
        if (!ifParallel(__direct)) {
            return false;
        }
        if (b() != 0) {
            return (c() / b()) == (__direct.c() / __direct.b());
        }
        if (a() != 0) {
            return (c() / a()) == (__direct.c() / __direct.a());
        }
        return false;
    }

    point cross (const direct &__direct) const
    {
        if (ifParallel(__direct)) {
            throw err_Parallel_directions();
        }
        coord_t y = (a() * __direct.c() - c() * __direct.a()) / (b() * __direct.a() - a() * __direct.b());
        coord_t x = (-1) * (c() + b() * y) / a();
        return point(x, y);
    }
};


// iostream
std::ostream &operator<< (std::ostream &st, const point &__point)
{
    return st << "(" << __point.x() << "; " << __point.y() << ")";
}
std::ostream &operator<< (std::ostream &st, const direct &__direct)
{
    if (__direct.a() != 0) {
        st << __direct.a() << "*x";
    }
    if (__direct.b() != 0) {
        if (__direct.a() != 0) {
            st << " + ";
        }
        st << __direct.b() << "*y";
    }
    if (__direct.c() != 0) {
        st << " + " << __direct.c();
    }
    return st << " = 0";
}

#endif // FIGURES_H_INCLUDED
