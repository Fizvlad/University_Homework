#include <iostream>
#include "figures.h"

using namespace std;

int main()
{
    Direct d1(1, 1, 1);
    Direct d2(2, 2, 2);
    Direct d3(1, 1, 0);
    Direct d4(1, 0, 5);
    Point point1(100, 0);
    cout << d1.ifParallel(d3) << endl;
    cout << d1.ifParallel(d4) << endl;
    cout << d1.ifEqual(d2) << endl;
    cout << d1.cross(d4) << endl;
    cout << d4.x(1337) << endl;
    cout << point1.x() << endl;
    cout << (point1 < d4) << endl;
    cout << Direct(Point(-1, 1), Point(1, 1)) << endl << endl;

    Polygon p1(4, Point(-1, -1), Point(1, -1), Point(1, 1), Point(-1 ,1));
    cout << p1.getDirect(0) << endl;
    cout << p1.getDirect(3) << endl;

    Polygon p2(4, Point(-2, 0), Point(0,0), Point(-1, 2), Point(-2, 1));
    cout << p2.ifPointLiesIn(Point(-100, 0)) << endl;
    cout << p2.ifPointLiesIn(Point(-2, 0.5), false) << endl;
    cout << p2.ifPointLiesIn(Point(-2, 0.5), true) << endl;
    cout << p2.ifPointLiesIn(Point(-1, 2), true) << endl;
    cout << p2.ifPointLiesIn(Point(-1, 1)) << endl;
    return 0;
}
