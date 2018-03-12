#include <iostream>

#include "Interpolation.h"

using namespace std;
using namespace interpolation;

int main()
{
    // Task 1
    std::vector<Point> points({
    {0.05, 20},
    {0.1, 10},
    {0.15, 6.6666},
    {0.2, 5},
    {0.25, 4},
    {0.3, 3.3333},
    {0.35, 2.8571},
    {0.4, 2.5},
    {0.45, 2.2222},
    {0.5, 2},
    });

    Polynomial polynomial = interpolate(points);
    cout << "Interpolation result:    " << polynomial << endl << endl;
    for (Point p : points) {
        cout << "f(" << p.x << ") = " << polynomial(p.x) << "; Correct value: " << p.y << endl;
    }
    cout << endl;
    cout << "y(0.28) = " << polynomial(0.28) << endl;
    cout << "y(0.475) = " << polynomial(0.475) << endl;

    return 0;
}
