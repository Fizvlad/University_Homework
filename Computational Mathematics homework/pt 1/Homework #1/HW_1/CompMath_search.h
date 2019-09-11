#ifndef COMPMATH_SEARCH_H_INCLUDED
#define COMPMATH_SEARCH_H_INCLUDED

/// Finds interval i : i in interval, predicate(i) = true, i.size <= step
/// Function returns [interval.end;interval.end] if can not find such subinterval
/// \param interval Search area
/// \param predicate Predicate to find matching interval
/// \param step Size of step
template <typename IntervalToBool_f_t> Interval findMatchingSubinterval (Interval interval, IntervalToBool_f_t predicate, double step) {
    if (step <= 0) {
        throw CompMath_Exception("Wrong step. Value is negative or equal to 0");
    }
    if (interval.begin.value > interval.end.value) {
        throw CompMath_Exception("Wrong interval");
    }

    Point x_0 = interval.begin;
    Point x_1(x_0.value + step);
    while (x_1.value <= interval.end.value) {
        Interval i(x_0, x_1);
        if (predicate(i)) {
            return i;
        }
        x_0 = x_1;
        x_1.value += step;
    }
    return Interval(interval.end, interval.end);
}

/// Function goes through interval with specified step and finds point with maximum value of valuer(point)
/// \param interval Search area
/// \param valuer Valuer
/// \param step Size of step
template <typename PointToDouble_f_t> Point findPointWithHighestValue (Interval interval, PointToDouble_f_t valuer, double step) {
    if (step <= 0) {
        throw CompMath_Exception("Wrong step. Value is negative or equal to 0");
    }
    if (interval.begin.value > interval.end.value) {
        throw CompMath_Exception("Wrong interval");
    }

    double maximum = valuer(interval.begin.value);
    Point point = interval.begin;
    for (double x = interval.begin.value + step; x <= interval.end.value; x += step) {
        double v = valuer(x);
        if (v > maximum) {
            maximum = v;
            point.value = x;
        }
    }
    return point;
}

#endif // COMPMATH_SEARCH_H_INCLUDED
