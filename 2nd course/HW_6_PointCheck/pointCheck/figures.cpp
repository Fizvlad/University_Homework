#include "figures.h"

//   Point
// Setters
coord_t point::x (const coord_t &__x)
{
    _x = __x;
    return _x;
}
coord_t point::y (const coord_t &__y)
{
    _y = __y;
    return _y;
}
void point::set (const coord_t &__x, const coord_t &__y)
{
    _x = __x;
    _y = __y;
}

// Operator=
point &point::operator= (const point &__point)
{
    set(__point.x(), __point.y());
    return *this;
}

// Constructor
point::point (coord_t __x, coord_t __y)
{
    set(__x, __y);
}

// 5. operator= locked. Copying default. Default destructor

// Getters
coord_t point::x () const
{
    return _x;
}
coord_t point::y () const
{
    return _y;
}

// Other
coord_t point::distance (const coord_t &__x, const coord_t &__y) const
{
    return std::sqrt(std::pow(x() - __x, 2) + std::pow(y() - __y, 2));
}
coord_t point::distance (const point &__point) const
{
    return distance(__point.x(), __point.y());
}

//    Direct
// Safety
void direct::_check () const
{
    if (_a == 0 && _b == 0) {
        throw err_Wrong_direct();
    }
}

// Setters
coord_t direct::a (const coord_t &__a)
{
    _a = __a;
    _check();
    return _a;
}
coord_t direct::b (const coord_t &__b)
{
    _b = __b;
    _check();
    return _b;
}
coord_t direct::c (const coord_t &__c)
{
    _c = __c;
    _check();
    return _c;
}
void direct::set (const coord_t &__a, const coord_t &__b, const coord_t &__c)
{
    _a = __a;
    _b = __b;
    _c = __c;
    _check();
}

// Operator=
direct &direct::operator= (const direct &__direct)
{
    set(__direct.a(), __direct.b(), __direct.c());
    return *this;
}

// Constructor
direct::direct (const coord_t &__a, const coord_t &__b, const coord_t &__c)
{
    set(__a, __b, __c);
}
direct::direct (const point &__point1, const point &__point2)
{
    coord_t a, b, c;
    a = __point1.y() - __point2.y();
    b = __point2.x() - __point1.x();
    c = __point1.x() * __point2.y() - __point2.x() * __point1.y();
    set(a, b, c);
}

// 5. operator= locked. Copying default. Default destructor

// Getters
coord_t direct::a () const
{
    return _a;
}
coord_t direct::b () const
{
    return _b;
}
coord_t direct::c () const
{
    return _c;
}

// Other
bool direct::ifParallel (const direct &__direct) const
{
    if (a() == 0 && __direct.a() == 0) {
        return true;
    }
    if (a() * __direct.a() == 0) {
        return false;
    }
    return (b() / a()) == (__direct.b() / __direct.a());
}
bool direct::ifEqual (const direct &__direct) const
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

point direct::cross (const direct &__direct) const
{
    if (ifParallel(__direct)) {
        throw err_Parallel_directions();
    }
    coord_t y = (a() * __direct.c() - c() * __direct.a()) / (b() * __direct.a() - a() * __direct.b());
    coord_t x = (-1) * (c() + b() * y) / a();
    return point(x, y);
}

coord_t direct::x (const coord_t &__y) const
{
    if (a() == 0) {
        if (__y != (-1) * c() / b()) {
            throw err_No_such_point_on_direct();
        }
        return 0;
    }
    return (-1) * (c() + b() * __y) / a();
}
coord_t direct::y (const coord_t &__x) const
{
    if (b() == 0) {
        if (__x != (-1) * c() / a()) {
            throw err_No_such_point_on_direct();
        }
        return 0;
    }
    return (-1) * (c() + a() * __x) / b();
}

//    Polygon
// Safety
void polygon::_check () const
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
            position = newPosition; // Changes nothing or changing from 0 to +-1
        }
    }
}

// Setters
void polygon::set (size_t __size, point __point, ...)
{
    if (__size < 3) {
        throw err_Not_polygon();
    }
    _points.resize(__size);
    for (size_t i = 0; i < __size; i++) {
        _points.at(i) = *(&__point + i);
    }
    _check();
}
void polygon::set (const std::vector <point> &__points)
{
    _points.resize(__points.size());
    for (size_t i = 0; i < _points.size(); i++) {
        _points.at(i) = __points.at(i);
    }
}

// Operator=
polygon &polygon::operator= (const polygon &__polygon)
{
    set(__polygon._points);
    return *this;
}

// Constructor
polygon::polygon (const size_t &__size, const point __point, ...)
{
    if (__size < 3) {
        throw err_Not_polygon();
    }
    _points.resize(__size);
    for (size_t i = 0; i < __size; i++) {
        _points.at(i) = *(&__point + i);
    }
    _check();
}

// 5. operator= locked. Copying not default cause of vectors. Default destructor
polygon::polygon (const polygon &__polygon)
{
    set(__polygon._points);
}

// Getters
size_t polygon::n () const
{
    return _points.size();
}
point polygon::getPoint (const size_t &__i) const
{
    return _points.at(__i);
}
direct polygon::getDirect (const size_t &__i) const
{
    return direct(_points.at(__i), _points.at((__i + 1) % n()));
}

// Other
bool polygon::ifPointLiesIn (const point &__point, const bool &__trueIfOnBorder) const // Return true if point is inside of figure or on border
{
    for (size_t i = 0; i < n(); i++) {
        direct d = getDirect(i); // Side between i and i + 1
        if (__point == d) {
            // Point lies on direct containing side. In this situation point can only be on side or out of borders
            if (!__trueIfOnBorder) { // Don't even need to check
                return false;
            }
            point p1 = getPoint(i);
            point p2 = getPoint((i + 1) % n());
            if ((d.b() != 0 && (std::abs(p1.x() - __point.x()) + std::abs(p2.x() - __point.x()) == std::abs(p2.x() - p1.x()))) || // Checked projections of points on oX (if vertical checking oY)
                ((d.b() == 0) && (std::abs(p1.y() - __point.y()) + std::abs(p2.y() - __point.y()) == std::abs(p2.y() - p1.y())))) { // Checked oY
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

// iostream
std::ostream &operator<< (std::ostream &st, const point &__point)
{
    return st << "(" << std::setprecision(FIGURES_COUT_PRECISION) << __point.x() << "; " << std::setprecision(FIGURES_COUT_PRECISION) << __point.y() << ")";
}
std::ostream &operator<< (std::ostream &st, const direct &__direct)
{
    if (__direct.a() != 0) {
        st << std::setprecision(FIGURES_COUT_PRECISION) << __direct.a() << "*x";
    }
    if (__direct.b() != 0) {
        if (__direct.a() != 0) {
            st << " + ";
        }
        st << std::setprecision(FIGURES_COUT_PRECISION) << __direct.b() << "*y";
    }
    if (__direct.c() != 0) {
        st << " + " << std::setprecision(FIGURES_COUT_PRECISION) << __direct.c();
    }
    return st << " = 0";
}

// Comparison
bool operator< (const point &__point, const direct &__direct)
{
    if (__direct.b() == 0) {
        return __point.x() > __direct.x(__point.y()); // point.y can be replaced with any number
    }
    return __point.y() < __direct.y(__point.x());
}
bool operator== (const point &__point, const direct &__direct)
{
    if (__direct.b() == 0) {
        return __point.x() == __direct.x(__point.y()); // point.y can be replaced with any number
    }
    return __point.y() == __direct.y(__point.x());
}
bool operator> (const point &__point, const direct &__direct)
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

// Exceptions
const char * err_Wrong_direct::what()
{
    return "Wrong initialization of direct";
}
const char * err_Parallel_directions::what()
{
    return "Parallel or equal directions given";
}
const char * err_No_such_point_on_direct::what()
{
    return "This direct (vertical or horizontal) do not have such point";
}
const char * err_Polygon_is_not_convex::what()
{
    return "Polygon is not convex";
}
const char * err_Not_polygon::what()
{
    return "Polygon must have at least 3 points";
}
const char * err_Points_in_line::what()
{
    return "Polygon have several points on one side";
}

