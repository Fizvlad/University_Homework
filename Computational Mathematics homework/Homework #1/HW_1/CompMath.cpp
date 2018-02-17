#include "CompMath.h"

// CompMath
Interval findDrawdown (Function<Consumer_Function_t> f, Interval interval, size_t depth) {
    double v_a = f.valueAt(interval.begin);
    double v_b = f.valueAt(interval.end);
    //std::cout << "Reqested interval : " << interval << ". Function values on edges: " << v_a << "; " << v_b << std::endl;
    if (f.valueAt(interval.begin) * f.valueAt(interval.end) <= 0) {
        return interval;
    } else if (depth > 0) {
        Point middle((interval.begin.value + interval.end.value) / 2);
        Interval left = findDrawdown(f, Interval(interval.begin, middle), depth - 1);
        if (left.size() == 0) {
            Interval right = findDrawdown(f, Interval(middle, interval.end), depth - 1);
            if (right.size() == 0) {
                return Interval(interval.end, interval.end);
            } else {
                return right;
            }
        } else {
            return left;
        }
    } else {
        return Interval(interval.end, interval.end);
    }
}

Point findSolution (Function<Consumer_Function_t> f, Interval interval) {
    return Point();
}
