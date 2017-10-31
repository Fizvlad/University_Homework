#include <iostream>
#include <cmath>
#include "figures.h"

using namespace std;

int main()
{
    direct d1();
    direct d2();
    direct d3(1, -1, 0);
    direct d4(0, 1, 0);
    direct d5(1, 0, 1);
    cout << d3.ifParallel(d3) << endl;
    cout << d5.ifEqual(d3) << endl;
    return 0;
}
