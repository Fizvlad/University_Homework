#ifndef INTERPOLATION_H_INCLUDED
#define INTERPOLATION_H_INCLUDED

#include <vector>
#include <iostream>
#include <cmath>

namespace interpolation {
    struct Point {
        double x;
        double y;

        Point (double X = 0, double Y = 0) : x(X), y(Y) {}
    };


    struct InterpolationPoint {
        double x;
        std::vector<double> f;

        InterpolationPoint (double X = 0, double F = 0) : x(X), f({F}) {}
        InterpolationPoint (double X, std::vector<double> F) : x(X), f(F) {}
    };


    struct Polynomial {
        std::vector<double> coefficient;

        Polynomial () : coefficient(1, 0) {}
        Polynomial (std::vector<double> Coefficient) : coefficient(Coefficient) {}
        Polynomial (double d) : coefficient({d}) {}

        unsigned deg () {return coefficient.size() - 1;}

        double operator() (const double& x) {
            double result = 0;
            for (size_t i = 0; i < coefficient.size(); i++) {
                result += coefficient[i] * std::pow(x, i);
            }
            return result;
        }


        Polynomial& operator*= (const Polynomial& p)
        {
            size_t l = coefficient.size() + p.coefficient.size() - 1;
            std::vector<double> newCoefficient(l, 0);
            for (size_t j = 0; j < coefficient.size(); j++) {
                for (size_t k = 0; k < p.coefficient.size(); k++) {
                    newCoefficient[j + k] += coefficient[j] * p.coefficient[k];
                }
            }
            coefficient = newCoefficient;
            return *this;
        }
    };

    Polynomial derivative (const Polynomial &p, unsigned d = 1) {
        std::vector<double> coefficient(p.coefficient.size() - d);
        for (size_t i = 0; i < p.coefficient.size() - d; i++) {
            coefficient[i] = p.coefficient[i + d];
            for (size_t j = 0; j < d; j++) {
                coefficient[i] *= i + d - j;
            }
        }
        return Polynomial(coefficient);
    }

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
        int factorial(int n)
        {
          return (n == 1 || n == 0) ? 1 : factorial(n - 1) * n;
        }


        double getDiff_ (std::vector<InterpolationPoint> input, unsigned depth = 0) {
            double result;
            if (input.size() == 0) {
                std::cerr << "Called getDiff_ from empty vector!" << std::endl;
                result = 0;
            } else if (input.size() == 1) {
                result = input[0].f[0];
            } else {
                bool ifPointsAreSame = true;
                for (InterpolationPoint p : input) {
                    if (p.x != input[0].x) {
                        ifPointsAreSame = false;
                        break;
                    }
                }

                if (ifPointsAreSame) {
                    unsigned n = input.size() - 1;
                    result = input[0].f[n]/ factorial(n);
                } else {
                    std::vector<InterpolationPoint> points1(input.begin() + 1, input.end());
                    std::vector<InterpolationPoint> points2(input.begin(), input.end() - 1);
                    result = (getDiff_(points1, depth + 1) - getDiff_(points2, depth + 1)) / (input[input.size() - 1].x - input[0].x);
                }
            }
            /*
            for (unsigned i = 0; i < depth; i++) {
                std::cout << " ";
            }
            std::cout << "Diff_" << input.size() << "_(" << input.begin()->x << "<>" << (input.end() - 1)->x << "): " << result << std::endl;
            */
            return result;
        }

        std::vector<double> getAllDiffs_ (std::vector<InterpolationPoint> input) {
            std::vector<double> result(input.size());
            for (size_t i = 0; i < result.size(); i++) {
                result[i] = getDiff_(std::vector<InterpolationPoint>(input.begin(), input.begin() + i + 1));
            }
            return result;
        }
    }
    Polynomial interpolate (std::vector<InterpolationPoint> input) {
        std::vector<InterpolationPoint> input_;
        for (InterpolationPoint p : input) {
            for (size_t i = 0; i < p.f.size(); i++) {
                input_.push_back(p);
            }
        }
        std::vector<double> diffs = getAllDiffs_(input_);
        Polynomial result;
        for (size_t i = 0; i < diffs.size(); i++) {
            Polynomial m({diffs[i]});
            for (size_t j = 0; j < i; j++) {
                m *= Polynomial({-1 * input_[j].x, 1});
            }
            result = result + m;
        }
        return result;
    }
}

#endif // INTERPOLATION_H_INCLUDED
