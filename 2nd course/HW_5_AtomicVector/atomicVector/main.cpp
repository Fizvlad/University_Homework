#include <iostream>
#include "atomicVector.h"

using namespace std;

int main()
{
    try {
        atomicVector <int> a;
        atomicVector <int> b(a);
        atomicVector <int> c(2, 0);
        c[0] = 10;
        c[1] = 30;
        c.push_before(2, 1);
        cout << c.size() << endl;
        cout << c << endl;
    } catch (const char* s) {
        cout << s;
    }
    return 0;
}
