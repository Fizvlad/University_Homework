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

// TODO Rework system of points, intervals and functions

template <typename Function_t> class BiFunctions {
public:
    Function_t first_valueAt;
    Function_t first_x_derivativeAt;
    Function_t first_y_derivativeAt;

    Function_t second_valueAt;
    Function_t second_x_derivativeAt;
    Function_t second_y_derivativeAt;

    BiFunctions (Function_t fv, Function_t fdx, Function_t fdy, Function_t sv, Function_t sdx, Function_t sdy) :
        first_valueAt(fv), first_x_derivativeAt(fdx), first_y_derivativeAt(fdy), second_valueAt(sv), second_x_derivativeAt(sdx), second_y_derivativeAt(sdy) {};
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


#endif // COMPMATH_UTILITY_H_INCLUDED
