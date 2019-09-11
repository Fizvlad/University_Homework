#include <iostream>

#include "CompMath_interpolation.h"

using namespace std;
using namespace interpolation;

int main()
{
    cout.precision(8);
    // Task 1
    vector<InterpolationPoint> points1 = {
        InterpolationPoint(0.05, 20),
        InterpolationPoint(0.1, 10),
        InterpolationPoint(0.15, 6.6666),
        InterpolationPoint(0.2, 5),
        InterpolationPoint(0.25, 4),
        InterpolationPoint(0.3, 3.3333),
        InterpolationPoint(0.35, 2.8571),
        InterpolationPoint(0.4, 2.5),
        InterpolationPoint(0.45, 2.2222),
        InterpolationPoint(0.5, 2)
    };
    Polynomial polynomial1 = interpolate(points1);
    cout << "Interpolation result:    " << polynomial1 << endl << endl;
    for (InterpolationPoint p : points1) {
        cout << "f(" << p.x << ") = " << polynomial1(p.x) << "; Correct value: " << p.f[0] << endl;
    }
    cout << endl;
    cout << "y(0.28) = " << polynomial1(0.28) << endl;
    cout << "y(0.475) = " << polynomial1(0.475) << endl;
    double y = 4.5;
    double x = findPreimage(4.5, {0.2, 0.25}, polynomial1); // ! Have to set interval 'with hands'
    cout << "x(" << y << ") = " << x << endl;
    cout << "f(" << x << ") = " << polynomial1(x) << "; Correct value: " << y << endl;

    cout << endl << endl;





    // Task 2
    vector<InterpolationPoint> points2 = {
        InterpolationPoint(0, {1, 2, 10, 36, 192}),
        InterpolationPoint(1, {36, 144}),
        InterpolationPoint(2, {1225, 3710})
    };

    size_t max_r = 5;
    std::vector<Polynomial> polynomials2(max_r);
    polynomials2[0] = interpolate(points2);
    for (size_t i = 0; i < max_r - 1; i++) {
        Polynomial d = derivative(polynomials2[i]);
        polynomials2[i + 1] = d;
    }
    cout << "Interpolation result:    ";
    for (Polynomial d : polynomials2) {
        cout << d << endl;
    }
    cout << endl;
    for (InterpolationPoint p : points2) {
        for (size_t j = 0; j < p.f.size() && j < polynomials2.size(); j++) {
            cout << "f";
            for (size_t k = 0; k < j; k++) {
                cout << "'";
            }
            cout << "(" << p.x << ") = " << polynomials2[j](p.x) << "; Correct value: " << p.f[j] << endl;
        }
    }

    return 0;
}
