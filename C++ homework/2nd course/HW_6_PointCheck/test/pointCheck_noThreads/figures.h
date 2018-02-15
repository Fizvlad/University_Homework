#ifndef FIGURES_H_INCLUDED
#define FIGURES_H_INCLUDED

#include <cmath> // abs, sqrt pow
#include <vector> // vector
#include <iostream> // cout
#include <iomanip> // setprecision

typedef float coord_t;

// Classes
class Point // (x, y)
{
    friend class Direct;
    friend class Polygon;

    coord_t _x;
    coord_t _y;

    // Setters
    coord_t x (const coord_t &__x);
    coord_t y (const coord_t &__y);
    void set (const coord_t &__x, const coord_t &__y);

    // Operator=
    Point &operator= (const Point &__point);

public:
    // Constructor
    Point (coord_t __x = 0, coord_t __y = 0);

    // 5. operator= locked. Copying default. Default destructor

    // Getters
    coord_t x () const;
    coord_t y () const;

    // Other
    coord_t distance (const coord_t &__x, const coord_t &__y) const; // Distance to coordinates
    coord_t distance (const Point &__point) const; // Distance to Point
};

class Direct // ax + by + c = 0
{
    friend class Polygon;

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
    Direct &operator= (const Direct &__direct);

public:
    // Constructor
    Direct (const coord_t &__a = 1, const coord_t &__b = -1, const coord_t &__c = 0); // Default: y=x
    Direct (const Point &__point1, const Point &__point2);

    // 5. operator= locked. Copying default. Default destructor

    // Getters
    coord_t a () const;
    coord_t b () const;
    coord_t c () const;

    // Other
    bool ifParallel (const Direct &__direct) const;
    bool ifEqual (const Direct &__direct) const;

    Point cross (const Direct &__direct) const; // Return Point where directs cross (throws error if parallel)

    coord_t x (const coord_t &__y) const; // Return X of Point on Direct with given Y
    coord_t y (const coord_t &__x) const; // Return Y of Point on Direct with given X
};

class Polygon // Described by N of points
{
    std::vector <Point> _points;

    // Safety
    void _check () const; // Polygon can not have nearby sides to be on same Direct. Polygon must be convex

    // Setters
    void set (size_t __size, Point __point, ...);
    void set (const std::vector <Point> &__points);

    // Operator=
    Polygon &operator= (const Polygon &__polygon);

public:
    // Constructor
    Polygon (const size_t &__size, const Point __point, ...); // Custom amount of points

    // 5. operator= locked. Copying not default cause of vectors. Default destructor
    Polygon (const Polygon &__polygon);

    // Getters
    size_t n () const; // Return amount of points
    Point getPoint (const size_t &__i) const; // Return points[i]
    Direct getDirect (const size_t &__i) const; // Returnes Direct through points[i] and points[i+1]

    // Other
    bool ifPointLiesIn (const Point &__point, const bool &__trueIfOnBorder = true) const; // Return true if Point is inside of figure or on border
};


// iostream
std::ostream &operator<< (std::ostream &st, const Point &__point); // (x; y)
std::ostream &operator<< (std::ostream &st, const Direct &__direct); // a * x + b * y + c = 0

// Comparison
// Same to comparing Y of Point and Y(Point.X) of Direct. If Direct is vertical Point<Direct when its on the right side
bool operator<  (const Point &__point, const Direct &__direct);
bool operator== (const Point &__point, const Direct &__direct);
bool operator>  (const Point &__point, const Direct &__direct);
bool operator<= (const Point &__point, const Direct &__direct);
bool operator>= (const Point &__point, const Direct &__direct);
bool operator!= (const Point &__point, const Direct &__direct);

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
