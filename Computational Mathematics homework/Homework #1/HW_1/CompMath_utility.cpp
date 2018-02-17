#include "CompMath_utility.h"

/*
    Utility classes
*/

Point::Point (double v) : value(v) {};
Point::operator double () {
    return value;
};

std::ostream &operator<< (std::ostream &s, Point p) {
    return s << p.value;
}


Interval::Interval (Point b, Point e) : begin(b), end(e) {}
double Interval::size () {
    return end.value - begin.value;
}

std::ostream &operator<< (std::ostream &s, Interval i) {
    return s << "[" << i.begin << ";" << i.end << "]";
}

/*
    Exceptions
*/
CompMath_Exception::CompMath_Exception(std::string s) : what_(s) {}
const char* CompMath_Exception::what() {
    return what_.c_str();
}
