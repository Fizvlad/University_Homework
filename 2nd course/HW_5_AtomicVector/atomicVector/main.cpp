#include <iostream>
#include "atomicVector.h"

using namespace std;

template <typename _T> void printData (atomicVector <_T> a) {
    cout << endl;
    cout << "Vector: " << a << endl;
    cout << "Begin:" << a.begin() << endl;
    cout << "End: " << a.end() << endl;
    cout << "Capacity: " << a.capacity() << endl;
}

int main()
{
    try {
        atomicVector <float> v(10, 1.337);
        v.push_before(1.11, 9);
        v.push_back(10.111);
        printData(v);
        cout << v.pop(10) << endl;
        cout << v.capacity() << endl;
        v.flip();
        printData(v);
        v.resize(2);
        printData(v);
        v.reserve(3);
        v[4] = 444;
        v.at(3) = 3333;
        printData(v);
    } catch (const char* s) {
        cout << s;
    }
    return 0;
}
