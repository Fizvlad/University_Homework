#include <iostream>
#include <thread>
#include <vector>
#include <atomic>
#include "customAtomic.h"

using namespace std;

void increase(customAtomic<int>* ptr, unsigned s)
{
	for (unsigned i = 0; i < s; i++) {
		(*ptr)++;
	}
}

int main()
{
	unsigned threadsAmount = 10;
	customAtomic <int> a(-1);
	increase(&a, 1);

	thread* arr = new thread[threadsAmount];
	for (unsigned i = 0; i < threadsAmount; i++) {
		arr[i] = thread(increase, &a, 100000);
		cout << i << " : " << a << endl;
	}
	for (unsigned i = 0; i < threadsAmount; i++) {
		arr[i].join();
	}
	cout << "Final : " << a.exchange(1234) << endl;
	cout << "New value : " << a.load() << endl;
	delete [] arr;
	return 0;
}
