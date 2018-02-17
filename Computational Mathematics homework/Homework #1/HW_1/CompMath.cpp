#include "CompMath.h"

// CompMath
Interval findDrawdown_iteratively (Function<Consumer_Function_t> f, Interval interval, size_t depth) {
    if (interval.begin.value > interval.end.value) {
        throw CompMath_Exception("Wrong interval");
    }
    double v_a = f.valueAt(interval.begin);
    double v_b = f.valueAt(interval.end);
    //std::cout << "Reqested interval : " << interval << ". Function values on edges: " << v_a << "; " << v_b << std::endl;

    if (f.valueAt(interval.begin) * f.valueAt(interval.end) <= 0) {
        return interval;
    } else if (depth > 0) {
        Point middle((interval.begin.value + interval.end.value) / 2);
        Interval left = findDrawdown_iteratively(f, Interval(interval.begin, middle), depth - 1);
        if (left.size() == 0) {
            Interval right = findDrawdown_iteratively(f, Interval(middle, interval.end), depth - 1);
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

// TODO function which goes through interval with given step and looking for interval by some predicate

Interval findDrawdown (Function<Consumer_Function_t> f, Interval interval, double step) {
    if (interval.begin.value > interval.end.value) {
        throw CompMath_Exception("Wrong interval");
    }
    //std::cout << "Reqested interval : " << interval << std::endl;

    Point x_0(interval.begin);
    Point x_1(interval.begin + step);
    while (x_1.value < interval.end.value) {
        if (f.valueAt(x_0) * f.valueAt(x_1) <= 0) {
            return Interval(x_0, x_1);
        }
        x_0.value = x_1.value;
        x_1.value = x_1.value + step;
    }
    return Interval(interval.end, interval.end);
}

Point findSolution (Function<Consumer_Function_t> f, Interval interval, double accuracy) {
    if (interval.begin.value > interval.end.value) {
        throw CompMath_Exception("Wrong interval");
    }
    interval = findDrawdown(f, interval);
    if (interval.size() == 0) {
        throw CompMath_Exception("Wrong interval");
    }
    Point x_0;
    if (std::abs(f.valueAt(interval.begin)) < std::abs(f.valueAt(interval.end))) {
        x_0.value = interval.begin;
    } else {
        x_0.value = interval.end;
    }
    Point x_1;
    double currentAccuracy;
    do {
        x_1.value = f.valueAt(x_0) + x_0;
        currentAccuracy = std::abs(x_1.value - x_0.value);
        x_0.value = x_1.value;
        std::cout << x_0 << std::endl;
    } while (currentAccuracy > accuracy);
    return x_1;
}
