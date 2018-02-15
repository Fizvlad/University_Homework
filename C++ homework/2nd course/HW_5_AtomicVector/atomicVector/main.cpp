#include <iostream>
#include <thread>
#include <mutex>
#include "atomicVector.h"

using namespace std;

template <typename _T> void printData (atomicVector <_T> a)
{
    cout << endl;
    cout << "Vector: " << a << endl;
    cout << "Begin:" << a.begin() << endl;
    cout << "End: " << a.end() << endl;
    cout << "Capacity: " << a.capacity() << endl;
}

template <typename _T> void func (atomicVector <_T> &v)
{
    v.push_back(v[v.size() - 1] + v[v.size() - 2]); // Sum of last two numbers
}
void _func (atomicVector <float> *v)
{
    func(*v);
}

int main()
{
    atomicVector <float> v(2);
    v[0] = 1;
    v[1] = 1;
    unsigned a = 20;
    thread *t = new thread[a];
    for (unsigned i = 0; i < a; i++) {
        t[i] = thread(_func, &v);
    }
    for (unsigned i = 0; i < a; i++) {
        t[i].join();
    }
    printData(v); // Must show Fibonacci
    return 0;
}
