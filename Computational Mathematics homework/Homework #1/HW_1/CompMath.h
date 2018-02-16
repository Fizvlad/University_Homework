#ifndef COMPMATH_H_INCLUDED
#define COMPMATH_H_INCLUDED

#include <string>
#include <exception>
#include <iostream>
#include <cmath>

typedef double (*doubleToDoubleFunc_t)(double);

class CompMath_Exception : std::exception {
    std::string what_;
public:
    CompMath_Exception(std::string s);
    const char* what();
};

double findDrawdown (doubleToDoubleFunc_t f, double a, double b, size_t depth = 10);

double findSolution (doubleToDoubleFunc_t f, double a, double b);

#endif // COMPMATH_H_INCLUDED
