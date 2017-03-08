#include <iostream>
#include "triad.h"

using namespace std;

void printT(triad a, string name) {
    cout << name << ": " << a.getNum(0) << " " << a.getNum(1) << " " << a.getNum(2) << endl;
}

int main()
{
    triad a;
    a.set(2, 4, 7);
    triad b(4, 8, 14);
    triad c(0,0,0);
    c = a + b;
    cout << "c = a + b" << endl;
    printT(a, "a");
    printT(b, "b");
    printT(c, "c");
    return 0;
}
