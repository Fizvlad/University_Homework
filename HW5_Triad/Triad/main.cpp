#include <iostream>
#include "triad.cpp"

using namespace std;

int main()
{
    triad <int, double, const char*> a;
    a.first() = 13;
    a.second() = 3.7;
    a.third() = "string";
    cout << a.first() << " " << a.second() << " " << a.third() << endl;
    triad <int, double, const char*> b(a);
    cout << b.first() << " " << b.second() << " " << b.third() << endl;
    triad <float, unsigned, char> (1.5, 2, 'c');
    return 0;
}
