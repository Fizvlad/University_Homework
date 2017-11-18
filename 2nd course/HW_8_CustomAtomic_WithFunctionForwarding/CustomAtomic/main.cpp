#include <iostream>
#include <thread>
#include <vector>
#include <atomic>
#include "customAtomic.h"

using namespace std;

struct myInt
{
    int i;
    int x2 () {return i * 2;};
    int xN (int N) {return i * N;}
    int pow2 () {return i * i;};
    myInt & operator++ () {i++; return *this;}

    myInt (int __i = 0) : i(__i) {}
};

ostream & operator<< (ostream & os, const myInt i) {
    return os << i.i;
}

void increase (customAtomic <myInt> * ptr, unsigned s)
{
	for (unsigned i = 0; i < s; i++) {
		(*ptr).call(myInt::operator++);
	}
}

int multipleBy (myInt i, int N)
{
    return i.xN(N);
}

int multipleBy3 (myInt i)
{
    return i.xN(3);
}

int main()
{
	unsigned threadsAmount = 10;
	customAtomic <myInt> a(-1); // We can only create customAtomic for objects with methods
	increase(&a, 1);

	thread* arr = new thread[threadsAmount];
	for (unsigned i = 0; i < threadsAmount; i++) {
		arr[i] = thread(increase, &a, 10000);
		cout << i << " : " << a << endl;
	}
	for (unsigned i = 0; i < threadsAmount; i++) {
		arr[i].join();
	}
	cout << "Final : " << a.exchange(1234) << endl;
	cout << "New value : " << a.load() << endl;
	cout << "x 2 : " << a.call(myInt::x2) << endl;
	cout << "x 4 : " << a.call(myInt::xN, 4) << endl;
	cout << "incremented : " << a.call(myInt::operator++) << endl;
	cout << "x 10 (apply) : " << a.apply(multipleBy, 10) << endl;
	cout << "x 5 (apply) : " << a.apply(multipleBy3) << endl;
	cout << "-1300 (lambda) : " << a.apply([] (myInt i) { return i.i - 1300;}) << endl;
	delete [] arr;
	return 0;
}
