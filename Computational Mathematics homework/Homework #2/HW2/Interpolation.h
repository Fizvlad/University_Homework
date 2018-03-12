#ifndef INTERPOLATION_H_INCLUDED
#define INTERPOLATION_H_INCLUDED

#include <vector>
#include <iostream>
#include <cmath>

namespace interpolation {
    struct Point {
        double x;
        double y;

        Point (double X, double Y) : x(X), y(Y) {}
    };


    struct Polynomial {
        std::vector<double> coefficient;

        Polynomial (): coefficient(1, 0) {}
        Polynomial (std::vector<double> Coefficient) : coefficient(Coefficient) {}
        Polynomial (double d) : coefficient({d}) {}

        unsigned deg () {return coefficient.size() - 1;}

        double operator() (const double& x) {
            double result = 0;
            for (size_t i = 0; i < coefficient.size(); i++) {
                result += coefficient[i] * std::pow(x, i);
            }
        }
    };

    std::ostream &operator<< (std::ostream &s, const Polynomial& p) {
        for (int i = p.coefficient.size() - 1; i >= 0; i--) {
            if (p.coefficient[i] == 0) {
                continue;
            }
            s << (p.coefficient[i] > 0 ? "+ " : "- ") << std::abs(p.coefficient[i]) << " * x^" << i << " ";
        }
        return s;
    }

    Polynomial operator+ (const Polynomial& left, const Polynomial& right) {
        size_t l = std::max(left.coefficient.size(), right.coefficient.size());
        std::vector<double> coefficient(l);
        for (size_t i = 0; i < l; i++) {
            coefficient[i] = (i < left.coefficient.size() ? left.coefficient[i] : 0) + (i < right.coefficient.size() ? right.coefficient[i] : 0);
        }
        return Polynomial(coefficient);
    }

    Polynomial operator- (const Polynomial& left, const Polynomial& right) {
        size_t l = std::max(left.coefficient.size(), right.coefficient.size());
        std::vector<double> coefficient(l);
        for (size_t i = 0; i < l; i++) {
            coefficient[i] = (i < left.coefficient.size() ? left.coefficient[i] : 0) - (i < right.coefficient.size() ? right.coefficient[i] : 0);
        }
        return Polynomial(coefficient);
    }

    Polynomial operator* (const Polynomial& left, const double& right) {
        size_t l = left.coefficient.size();
        std::vector<double> coefficient(l);
        for (size_t i = 0; i < l; i++) {
            coefficient[i] = right * left.coefficient[i];
        }
        return Polynomial(coefficient);
    }

    Polynomial operator* (const Polynomial& left, const Polynomial& right) {
        size_t l = left.coefficient.size() + right.coefficient.size() - 1;
        std::vector<double> coefficient(l, 0);
        for (size_t j = 0; j < left.coefficient.size(); j++) {
            for (size_t k = 0; k < right.coefficient.size(); k++) {
                coefficient[j + k] += left.coefficient[j] * right.coefficient[k];
            }
        }
        return Polynomial(coefficient);
    }


    namespace {
        // Utility namespace for interpolate

        double getDiff_ (std::vector<Point> points) {
            if (points.size() == 0) {
                return 0;
            } else if (points.size() == 1) {
                return points[0].y;
            } else {
                std::vector<Point> points1(points.begin() + 1, points.end());
                std::vector<Point> points2(points.begin(), points.end() - 1);
                return (getDiff_(points1) - getDiff_(points2)) / (points[points.size() - 1].x - points[0].x);
            }
        }

        std::vector<double> getAllDiffs_ (std::vector<Point> points) {
            std::vector<double> result(points.size());
            for (size_t i = 0; i < result.size(); i++) {
                result[i] = getDiff_(std::vector<Point>(points.begin(), points.begin() + i + 1));
            }
            return result;
        }
    }
    Polynomial interpolate (std::vector<Point> points) {
        std::vector<double> diffs = getAllDiffs_(points);
        Polynomial result;
        for (size_t i = 0; i < points.size(); i++) {
            Polynomial m({diffs[i]});
            for (size_t j = 0; j < i; j++) {
                m = m * Polynomial({-1 * points[j].x, 1});
            }
            result = result + m;
        }
        return result;
    }
}

#endif // INTERPOLATION_H_INCLUDED
