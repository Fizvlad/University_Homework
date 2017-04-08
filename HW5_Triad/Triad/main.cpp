#include <iostream>
#include "triad.h"

using namespace std;

int main()
{
    triad <int, double, const char*> a;
    a.first() = 13;
    a.second() = 3.7;
    a.third() = "string";
    cout << a.first() << " " << a.second() << " " << a.third() << endl; // 13 3.7 string

    triad <int, double, const char*> b(a);
    b.first() = 666;
    cout << b.first() << " " << b.second() << " " << b.third() << endl; // 666 3.7 string

    triad <float, unsigned, char> c(1.5, 2, 'c');
    cout << c.first() << " " << c.second() << " " << c.third() << endl; // 1.5 2 'c'
    c.set(2.5, 1, '\2');
    cout << c.first() << " " << c.second() << " " << c.third() << endl; // 2.5 1 /Smile/

    b.swap(a);
    cout << a.first() << " " << a.second() << " " << a.third() << endl; // 666 3.7 string
    cout << b.first() << " " << b.second() << " " << b.third() << endl; // 13 3.7 string
    a = b;
    cout << a.first() << " " << a.second() << " " << a.third() << endl; // 13 3.7 string
    b.swap(b);
    cout << b.first() << " " << b.second() << " " << b.third() << endl; // 13 3.7 string
    return 0;
}
