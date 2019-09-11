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
    if (accuracy <= 0) {
        throw CompMath_Exception("Wrong accuracy. Value is negative or equal to 0");
    }
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

Point findSolution_Newton (Function<Consumer_Function_t> f, Interval interval, double accuracy) {
    if (accuracy <= 0) {
        throw CompMath_Exception("Wrong accuracy. Value is negative or equal to 0");
    }
    double step = 1e-1; // Maximum of interval size
    interval = findDrawdown(f, interval, step);
    if (interval.size() == 0) {
        throw CompMath_Exception("Wrong interval. No drawdowns");
    }

    Point x_0 = interval.begin;
    Point x_1;
    double currentAccuracy;
    do {
        x_1.value = x_0 - f.valueAt(x_0) / f.derivativeAt(x_0);
        currentAccuracy = std::abs(x_1.value - x_0.value);
        x_0.value = x_1.value;
    } while (currentAccuracy * 10 >= accuracy); // Multiplying by 10 to get accurate last digit
    return x_1;
}


// TODO Figure out how to find search area
std::pair<Point, Point> findSolution_Newton (BiFunctions<Biconsumer_Function_t> f, double x, double y, double accuracy) {
    if (accuracy <= 0) {
        throw CompMath_Exception("Wrong accuracy. Value is negative or equal to 0");
    }

    Point x_0(x), y_0(y);
    Point x_1, y_1;
    double currentAccuracy;
    do {
        double d = f.first_x_derivativeAt(x_0, y_0) * f.second_y_derivativeAt(x_0, y_0) - f.second_x_derivativeAt(x_0, y_0) * f.first_y_derivativeAt(x_0, y_0);
        x_1.value = x_0 - (f.first_valueAt(x_0, y_0) * f.second_y_derivativeAt(x_0, y_0) - f.second_valueAt(x_0, y_0) * f.first_y_derivativeAt(x_0, y_0)) / d;
        y_1.value = y_0 - (f.first_x_derivativeAt(x_0, y_0) * f.second_valueAt(x_0, y_0) - f.second_x_derivativeAt(x_0, y_0) * f.first_valueAt(x_0, y_0)) / d;
        currentAccuracy = std::max(std::abs(x_1.value - x_0.value), std::abs(y_1.value - y_0.value));
        x_0.value = x_1.value;
        y_0.value = y_1.value;
    } while (currentAccuracy * 10 >= accuracy); // Multiplying by 10 to get accurate last digit
    return std::pair<Point, Point>(x_1, y_1);
}
