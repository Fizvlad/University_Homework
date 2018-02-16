#include "CompMath.h"


CompMath_Exception::CompMath_Exception(std::string s) : what_(s) {}
const char* CompMath_Exception::what() {return what_.c_str();}

/// Finds point x_0: x_0 != b, f(x_0) * f(b) <= 0.
/// Function returns b if can not find such point
// TODO Reowrk function so it returns pair of doubles
double findDrawdown (doubleToDoubleFunc_t f, double a, double b, size_t depth) {
    std::cout << "Reqested interval : " << "[" << a << ";" << b << "]" << ". Function values on edges: " << f(a) << "; " << f(b) << std::endl;
    double result;
    if (f(a) * f(b) <= 0) {
        result = a; // a is RESULT
    } else if (depth > 0) {
        // Splitting [a; b] to [a; a+b / 2] and [a+b / 2; b]
        double c = (a + b) / 2;
        double left = findDrawdown(f, a, c, depth - 1); // Looking for answer on left part
        if (left == c) {
            // No such point on left
            double right = findDrawdown(f, c, b, depth - 1); // Looking for answer on right part
            if (right == b) {
                // No such point on right
                result = b; // b is RESULT
            } else {
                result = right; // right is RESULT
            }
        } else {
            result = left; // left is RESULT
        }
    } else {
        result = b; // b is RESULT
    }
}

double findSolution (doubleToDoubleFunc_t f, double a, double b) {
    return 0;
}
