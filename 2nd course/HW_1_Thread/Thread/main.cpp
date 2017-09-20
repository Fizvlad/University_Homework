#include <iostream>
#include <iomanip>

#include <thread>
#include <vector>
#include <algorithm> // Sort

#include <ctime> // Rand
#include <cstdlib> // Rand

#include <functional> // Ref

void sortArr (std::vector <int> &arr)
{
    std::cout << "Sorting array. Thread id:" << std::this_thread::get_id() << std::endl;
    std::sort(arr.begin(), arr.end());
    std::cout << "Sorted" << std::endl;
}

void print (std::vector < std::vector <int> > v, int outWidth = 5)
{
    for (unsigned i = 0; i < v.size(); i++) {
        for (unsigned j = 0; j < v[i].size(); j++) {
            std::cout << std::setw(outWidth) << v[i][j] << " ";
        }
        std::cout << std::endl;
    }
}

int main ()
{
    // Constants
    int ARR_SIZE = 5;
    int ARR_AMOUNT = 5;
    int MAX_RANDOM = 100;

    // Initializing vector and reserving memory
    std::vector < std::vector <int> > arr;

    // Filling arrays
    std::srand(std::time(0));
    for (int i = 0; i < ARR_AMOUNT; i++) {
        std::vector <int> arr_i; // Creating array
        for (int j = 0; j < ARR_SIZE; j++) {
            arr_i.push_back(std::rand() % MAX_RANDOM); // Filling
        }
        arr.push_back(arr_i); // Adding to main vector
    }
    std::cout << "Created arrays: " << std::endl;
    print(arr, 3);
    std::cout << std::endl;

/*
    //Sorting massive with "for"

    for (int i = 0; i < ARR_AMOUNT; i++) {
        sortArr(arr[i]);
    }
*/

    // Creating and starting threads
    for (int i = 0; i < ARR_AMOUNT; i++) {
        std::thread t(sortArr, std::ref(arr[i]));
        // Using ref(arr[i]) because thread t(...) passing copies of array
        std::cout << "thread.join()\n"; // Using '\n' because endl can cause interesting bug when newline printed after t.join() ????
        t.join();
    }

    std::cout << std::endl;
    std::cout << "Sorted arrays: " << std::endl;
    print(arr, 3);

    return 0;
}
