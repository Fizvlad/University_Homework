#include <iostream>
#include <ctime>
#include <cstdlib>
#include <vector>
#include "Mergesort_Threads.cpp"

using namespace std;

int main()
{
    unsigned vector_size = 20;
    vector <int> arr;

    srand(time(0));
    for (unsigned i = 0; i < vector_size; i++) {
        arr.push_back(rand() % 1337);
    }
    cout << "Created array: " << endl;
    for (unsigned i = 0; i < arr.size(); i++) {
        cout << arr[i] << " ";
    }
    cout << endl;

    Mergesort(arr);

    cout << "Sorted array: " << endl;
    for (unsigned i = 0; i < arr.size(); i++) {
        cout << arr[i] << " ";
    }
    cout << endl;
    return 0;
}
