#ifndef COMPMATH_UTILITY_H_INCLUDED
#define COMPMATH_UTILITY_H_INCLUDED

#include <exception> // std::exception
#include <iostream> // i/o/e streams

/*
    Utility classes
*/

template <typename Function_t> class Function {
public:
    Function_t valueAt;
    Function_t derivativeAt;

    Function (Function_t v, Function_t d) : valueAt(v), derivativeAt(d) {};
};

class Point {
public:
    double value;

    Point (double v = 0);
    operator double ();
};

std::ostream &operator<< (std::ostream &s, Point p);


class Interval {
public:
    Point begin;
    Point end;

    Interval (Point b = 0, Point e = 0);

    double size ();
};

std::ostream &operator<< (std::ostream &s, Interval i);


/*
    Exceptions
*/

class CompMath_Exception : std::exception {
public:
    CompMath_Exception(std::string s);
    const char* what();
private:
    std::string what_;
};


/*
    Other
*/

template <typename T> int sgn (T val) {
    return (T(0) < val) - (val < T(0));
}

#endif // COMPMATH_UTILITY_H_INCLUDED
