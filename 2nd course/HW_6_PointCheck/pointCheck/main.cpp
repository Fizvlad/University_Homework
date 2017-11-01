#include <iostream>
#include <cmath>
#include "figures.h"

using namespace std;

int main()
{
    direct d1(1, 1, 1);
    direct d2(2, 2, 2);
    direct d3(1, 1, 0);
    direct d4(1, 0, 5);
    cout << d1.ifParallel(d3) << endl;
    cout << d1.ifParallel(d4) << endl;
    cout << d1.ifEqual(d2) << endl;
    cout << d1.cross(d4) << endl;
    cout << d2 << endl;
    cout << direct(point(-1, 0),point(1, 0));
    return 0;
}
