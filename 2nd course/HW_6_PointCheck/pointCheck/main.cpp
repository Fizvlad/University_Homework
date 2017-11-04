#include <iostream>
#include <cmath>
#include "figures.h"

using namespace std;

int main()
{
    direct d0();
    direct d1(1, 1, 1);
    direct d2(2, 2, 2);
    direct d3(1, 1, 0);
    direct d4(1, 0, 5);
    point point1(100, 0);
    cout << d1.ifParallel(d3) << endl;
    cout << d1.ifParallel(d4) << endl;
    cout << d1.ifEqual(d2) << endl;
    cout << d1.cross(d4) << endl;
    cout << d4.x(1337) << endl;
    cout << point1.x() << endl;
    cout << (point1 < d4) << endl;
    cout << direct(point(-1, 1), point(1, 1)) << endl << endl;

    polygon p1(4, point(-1, -1), point(1, -1), point(1, 1), point(-1 ,1));
    cout << p1.getDirect(0) << endl;
    cout << p1.getDirect(3) << endl;

    polygon p2(4, point(-2, 0), point(0,0), point(-1, 2), point(-1, 0.5));
    return 0;
}
