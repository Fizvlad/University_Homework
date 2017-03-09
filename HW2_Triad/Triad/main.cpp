#include <iostream>
#include "triad.h"

using namespace std;

void printT(triad& a, string name) {
    cout << name << ": " << a.getNum(0) << " " << a.getNum(1) << " " << a.getNum(2) << endl;
}

int main()
{
    triad a(1, 2, 3);
    printT(a, "a");
    triad b(a);
    b = b - a;
    printT(b, "b");
    triad c(0, 0, 0);
    c = 2 * a * 2 + b;
    printT(a, "a");
    printT(c, "c");
    return 0;
}
