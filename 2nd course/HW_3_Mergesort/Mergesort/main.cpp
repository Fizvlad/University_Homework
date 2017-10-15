#include <iostream>
#include <ctime>
#include <cstdlib>
#include <vector>
#include "Mergesort_threads.h"

using namespace std;

int main()
{
    unsigned vector_size = 20;
    unsigned maximumThreads = 20;
    vector <int> arr(vector_size);

    srand(time(0));
    for (unsigned i = 0; i < arr.size(); i++) {
        arr[i] = rand() % 1337;
    }
    cout << "Created array: " << endl;
    for (unsigned i = 0; i < arr.size(); i++) {
        cout << arr[i] << " ";
    }
    cout << endl;

    Mergesort(arr, maximumThreads);

    cout << "Sorted array: " << endl;
    for (unsigned i = 0; i < arr.size(); i++) {
        cout << arr[i] << " ";
    }
    cout << endl;
    return 0;
}
