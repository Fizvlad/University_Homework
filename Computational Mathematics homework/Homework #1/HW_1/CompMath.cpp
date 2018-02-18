#include "CompMath.h"

// CompMath
Interval findDrawdown_iteratively (Function<Consumer_Function_t> f, Interval interval, size_t depth) {
    if (interval.begin.value > interval.end.value) {
        throw CompMath_Exception("Wrong interval");
    }

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

Interval findDrawdown (Function<Consumer_Function_t> f, Interval interval, double step) {
    return findMatchingSubinterval(interval, [f](Interval i){return f.valueAt(i.begin) * f.valueAt(i.end) <= 0;}, step);
}

Point findSolution_SimpleIterations (Function<Consumer_Function_t> f, Interval interval, double accuracy) {
    double step = 1e-1; // Maximum of interval size
    interval = findDrawdown(f, interval, step);
    if (interval.size() == 0) {
        throw CompMath_Exception("Wrong interval. No drawdowns");
    }

    double maximumDerivative = f.derivativeAt(findPointWithHighestValue(interval, [f](Point p){return f.derivativeAt(p);}, step / 128));
    Point x_0 = interval.begin;
    Point x_1;
    double currentAccuracy;
    do {
        x_1.value = x_0 - f.valueAt(x_0) / maximumDerivative;
        currentAccuracy = std::abs(x_1.value - x_0.value);
        x_0.value = x_1.value;
    } while (currentAccuracy * 10 >= accuracy); // Multiplying by 10 to get accurate last digit
    return x_1;
}
