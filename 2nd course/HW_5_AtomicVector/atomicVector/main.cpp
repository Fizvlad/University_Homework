#include <iostream>
#include <thread>
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
    cout << "Thread id: " << this_thread::get_id() << endl;
    v.push_front(1.11);
    v.push_back(10.111);
    printData(v);
    cout << v.pop_back() << endl;
    cout << v.capacity() << endl;
    v.flip();
    printData(v);
    v.shrink_to_fit();
    printData(v);
    v.reserve(3);
    v[4] = 444;
    v.at(3) = 3333;
    printData(v);
}
void _func (atomicVector <float> *v)
{
    func(*v);
}

int main()
{
    atomicVector <float> v(10, 1.337);
    unsigned a = 4;
    thread *t = new thread[a];
    for (unsigned i = 0; i < a; i++) {
        t[i] = thread(_func, &v);
    }
    for (unsigned i = 0; i < a; i++) {
        t[i].join();
    }
    return 0;
}
