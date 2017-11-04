#ifndef FIGURES_H_INCLUDED
#define FIGURES_H_INCLUDED

#include <cmath>
#include <vector>
#include <iostream>

typedef float coord_t;

// Classes
class point;
class direct;
class polygon;

// iostream
std::ostream &operator<< (std::ostream &st, const point &__point);
std::ostream &operator<< (std::ostream &st, const direct &__direct);

// Comparison
bool operator< (const point &__point, const direct &__direct);
bool operator== (const point &__point, const direct &__direct);
bool operator> (const point &__point, const direct &__direct);
bool operator<= (const point &__point, const direct &__direct);
bool operator>= (const point &__point, const direct &__direct);
bool operator!= (const point &__point, const direct &__direct);

// Exceptions
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
struct err_Polygon_is_not_convex : std::exception
{
    const char* what()
    {
        return "Polygon is not convex";
    }
};
struct err_Not_polygon : std::exception
{
    const char* what()
    {
        return "Polygon must have at least 3 points";
    }
};
struct err_Points_in_line : std::exception
{
    const char* what()
    {
        return "Polygon have several points on one side";
    }
};


class point // (x, y)
{
    friend class direct;
    friend class polygon;

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

public:
    // Constructor
    point (coord_t __x = 0, coord_t __y = 0)
    {
        set(__x, __y);
    }

    // 5. operator= locked. Copying default. Default destructor

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
    friend class polygon;

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

public:
    // Constructor
    direct (const coord_t &__a, const coord_t &__b, const coord_t &__c)
    {
        set(__a, __b, __c);
    }
    direct (const point &__point1, const point &__point2)
    {
        coord_t a, b, c;
        a = __point1.y() - __point2.y();
        b = __point2.x() - __point1.x();
        c = __point1.x() * __point2.y() - __point2.x() * __point1.y();
        set(a , b, c);
    }

    // 5. operator= locked. Copying default. Default destructor

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

    coord_t y (const coord_t &__x) const
    {
        if (b() == 0) {
            return 0;
        }
        return (-1) * (c() + a() * __x) / b();
    }
    coord_t x (const coord_t &__y) const
    {
        if (a() == 0) {
            return 0;
        }
        return (-1) * (c() + b() * __y) / a();
    }
};

class polygon // Described by N of points
{
    std::vector <point> _points;

    // Safety
    void _check () const
    {
        for (size_t i = 0; i < n(); i++) {
            direct d = getDirect(i); // Direct connecting i and i+1
            short position = 0;// -1 if below, 1 if above, 0 initially
            for (size_t j = 0; j < n() - 2; j++) {
                point p = getPoint((i + 2 + j) % n()); // Taking point right after points related to side
                if (p == d) {
                    throw err_Points_in_line();
                }
                short newPosition = -1 + 2 * (p > d); // -1 if below, 1 if above
                if (position != 0 && position != newPosition) {
                    // position == 0 at first check
                    throw err_Polygon_is_not_convex();
                }
            }
        }
    }

    // Setters
    void set (size_t __size, point __point, ...)
    {
        if (__size < 3) {
            throw err_Not_polygon();
        }
        _points.resize(__size);
        for (size_t i = 0; i < __size; i++) {
            _points[i] = *(&__point + i);
        }
        _check();
    }
    void set (const std::vector <point> &__points)
    {
        _points.resize(__points.size());
        for (size_t i = 0; i < _points.size(); i++) {
            _points[i] = __points[i];
        }
    }

    // Operator=
    polygon &operator= (const polygon &__polygon)
    {
        set(__polygon._points);
        return *this;
    }

public:
    // Constructor
    polygon (const size_t &__size, const point &__point, ...)
    {
        if (__size < 3) {
            throw err_Not_polygon();
        }
        _points.resize(__size);
        for (size_t i = 0; i < __size; i++) {
            _points[i] = *(&__point + i);
        }
        _check();
    }

    // 5. operator= locked. Copying not default cause of vectors. Default destructor
    polygon (const polygon &__polygon)
    {
        set(__polygon._points);
    }

    // Getters
    size_t n () const
    {
        return _points.size();
    }
    point getPoint (const size_t &__i) const
    {
        return _points.at(__i);
    }
    direct getDirect (const size_t &__i) const
    {
        return direct(_points.at(__i), _points.at((__i + 1) % _points.size()));
    }

    // Other
    bool ifLiesIn (const point &__point, const bool &__trueIfOnBorder = true) const // Return true if point is inside of figure or on border
    {
        for (size_t i = 0; i < n(); i++) {
            direct d = getDirect(i); // Side between i and i + 1
            if (__point == d) {
                // Point lies on direct containing side. In this situation point can only be on side or out of borders
                if (!__trueIfOnBorder) { // Don't even need to check
                    return false;
                }
                point p1 = getPoint(i);
                point p2 = getPoint(i + 1);
                if ((d.b() != 0 && (std::abs(p1.x() - __point.x()) + std::abs(p2.x() - __point.x()) == std::abs(p2.x() - p1.x()))) || // Checked projections of points on oX (if vertical checking oY)
                    (std::abs(p1.y() - __point.y()) + std::abs(p2.y() - __point.y()) == std::abs(p2.y() - p1.y()))) { // Checked oY
                    return true;
                }
                return false; // Not on side
            } else {
                short pointPosition = -1 + 2 * (__point > d); // -1 if below, 1 if above
                short rightPosition = -1 + 2 * (getPoint((i + 2) % n()) > d); // Checking where is not relevant to direct point on polygon
                if (pointPosition != rightPosition) {
                    return false;
                }
            }
        }
        return true;
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

// Comparison
bool operator< (const point &__point, const direct &__direct) // True if below or on the right side
{
    if (__direct.b() == 0) {
        return __point.x() > __direct.x(__point.y()); // point.y can be replaced with any number
    }
    return __point.y() < __direct.y(__point.x());
}
bool operator== (const point &__point, const direct &__direct) // True if point lies on direct
{
    if (__direct.b() == 0) {
        return __point.x() == __direct.x(__point.y()); // point.y can be replaced with any number
    }
    return __point.y() == __direct.y(__point.x());
}
bool operator> (const point &__point, const direct &__direct) // True if point lies on direct
{
    return !(__point == __direct || __point < __direct);
}
bool operator<= (const point &__point, const direct &__direct)
{
    return !(__point > __direct);
}
bool operator>= (const point &__point, const direct &__direct)
{
    return !(__point < __direct);
}
bool operator!= (const point &__point, const direct &__direct)
{
    return !(__point == __direct);
}

#endif // FIGURES_H_INCLUDED
