#ifndef COMPMATH_H_INCLUDED
#define COMPMATH_H_INCLUDED

#include <string> // std::string
#include <cmath> // Math
#include <utility> // std::pair

#include "CompMath_utility.h"
#include "CompMath_search.h"

typedef double (*Consumer_Function_t)(double);
typedef double (*Biconsumer_Function_t)(double, double);

/*
    CompMath
*/
/// Finds some interval [x_0; x_1]: f(x_0) * f(x_1) <= 0, [x_0; x_1] in interval
/// Function returns [interval.end;interval.end] if can not find such point
/// \param f Function double -> double
/// \param interval Search area
/// \param depth Level of splitting
Interval findDrawdown_iteratively (Function<Consumer_Function_t> f, Interval interval, size_t depth = 10);

/// Finds some interval [x_0; x_1]: f(x_0) * f(x_1) <= 0, [x_0; x_1] in interval
/// Function returns [interval.end;interval.end] if can not find such point
/// \param f Function double -> double
/// \param interval Search area
/// \param step Step size
Interval findDrawdown (Function<Consumer_Function_t> f, Interval interval, double step);

/// Finds some solution in [a; b]
/// \param f Function double -> double
/// \param interval Search area
/// \param accuracy Amount of correct digits after decimal point
Point findSolution_SimpleIterations (Function<Consumer_Function_t> f, Interval interval, double accuracy);

/// Finds some solution in [a; b]
/// \param f Function double -> double
/// \param interval Search area
/// \param accuracy Amount of correct digits after decimal point
Point findSolution_Newton (Function<Consumer_Function_t> f, Interval interval, double accuracy);


/*
    CompMath. System of two equasions
*/
/// Finds some solution
/// \param f Two functions: double, double -> double
/// \param accuracy Amount of correct digits after decimal point
std::pair<Point, Point> findSolution_Newton (BiFunctions<Biconsumer_Function_t> f, double x, double y, double accuracy);


#endif // COMPMATH_H_INCLUDED
