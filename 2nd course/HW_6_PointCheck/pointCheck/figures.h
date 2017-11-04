#ifndef FIGURES_H_INCLUDED
#define FIGURES_H_INCLUDED

#include <cmath> // abs, sqrt pow
#include <vector> // vector
#include <iostream> // cout
#include <iomanip> // setprecision

typedef float coord_t;

size_t FIGURES_COUT_PRECISION = 3; // Precision of output to stream

// Classes
class point // (x, y)
{
    friend class direct;
    friend class polygon;

    coord_t _x;
    coord_t _y;

    // Setters
    coord_t x (const coord_t &__x);
    coord_t y (const coord_t &__y);
    void set (const coord_t &__x, const coord_t &__y);

    // Operator=
    point &operator= (const point &__point);

public:
    // Constructor
    point (coord_t __x = 0, coord_t __y = 0);

    // 5. operator= locked. Copying default. Default destructor

    // Getters
    coord_t x () const;
    coord_t y () const;

    // Other
    coord_t distance (const coord_t &__x, const coord_t &__y) const; // Distance to coordinates
    coord_t distance (const point &__point) const; // Distance to point
};

class direct // ax + by + c = 0
{
    friend class polygon;

    coord_t _a;
    coord_t _b;
    coord_t _c;

    // Safety
    void _check () const; // a*a + b*b != 0

    // Setters
    coord_t a (const coord_t &__a);
    coord_t b (const coord_t &__b);
    coord_t c (const coord_t &__c);
    void set (const coord_t &__a, const coord_t &__b, const coord_t &__c);

    // Operator=
    direct &operator= (const direct &__direct);

public:
    // Constructor
    direct (const coord_t &__a = 1, const coord_t &__b = -1, const coord_t &__c = 0); // Default: y=x
    direct (const point &__point1, const point &__point2);

    // 5. operator= locked. Copying default. Default destructor

    // Getters
    coord_t a () const;
    coord_t b () const;
    coord_t c () const;

    // Other
    bool ifParallel (const direct &__direct) const;
    bool ifEqual (const direct &__direct) const;

    point cross (const direct &__direct) const; // Return point where directs cross (throws error if parallel)

    coord_t x (const coord_t &__y) const; // Return X of point on direct with given Y
    coord_t y (const coord_t &__x) const; // Return Y of point on direct with given X
};

class polygon // Described by N of points
{
    std::vector <point> _points;

    // Safety
    void _check () const; // Polygon can not have nearby sides to be on same direct. Polygon must be convex

    // Setters
    void set (size_t __size, point __point, ...);
    void set (const std::vector <point> &__points);

    // Operator=
    polygon &operator= (const polygon &__polygon);

public:
    // Constructor
    polygon (const size_t &__size, const point __point, ...); // Custom amount of points

    // 5. operator= locked. Copying not default cause of vectors. Default destructor
    polygon (const polygon &__polygon);

    // Getters
    size_t n () const; // Return amount of points
    point getPoint (const size_t &__i) const; // Return points[i]
    direct getDirect (const size_t &__i) const; // Returnes direct through points[i] and points[i+1]

    // Other
    bool ifPointLiesIn (const point &__point, const bool &__trueIfOnBorder = true) const; // Return true if point is inside of figure or on border
};


// iostream
std::ostream &operator<< (std::ostream &st, const point &__point); // (x; y)
std::ostream &operator<< (std::ostream &st, const direct &__direct); // a * x + b * y + c = 0

// Comparison
// Same to comparing Y of point and Y(point.X) of direct. If direct is vertical point<direct when its on the right side
bool operator<  (const point &__point, const direct &__direct);
bool operator== (const point &__point, const direct &__direct);
bool operator>  (const point &__point, const direct &__direct);
bool operator<= (const point &__point, const direct &__direct);
bool operator>= (const point &__point, const direct &__direct);
bool operator!= (const point &__point, const direct &__direct);

// Exceptions
struct err_Wrong_direct : std::exception
{
    const char * what();
};
struct err_Parallel_directions : std::exception
{
    const char * what();
};
struct err_No_such_point_on_direct : std::exception
{
    const char * what();
};
struct err_Polygon_is_not_convex : std::exception
{
    const char * what();
};
struct err_Not_polygon : std::exception
{
    const char * what();
};
struct err_Points_in_line : std::exception
{
    const char * what();
};

#endif // FIGURES_H_INCLUDED
