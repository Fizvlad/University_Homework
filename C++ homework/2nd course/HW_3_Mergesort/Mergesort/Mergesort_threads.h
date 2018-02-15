#ifndef MERGESORT_THREADS_H_INCLUDED
#define MERGESORT_THREADS_H_INCLUDED

#include <iostream>
#include <vector>
#include <thread>

template <typename _T> void Mergesort (std::vector <_T> &arr, int maximumThreads = 0)
{
    // I hope this type have < method
    if (arr.size() == 1) {
        // Not changing
    } else if (arr.size() == 2) {
        if (arr[1] < arr[0]) {
            _T tmp = arr[1];
            arr[1] = arr[0];
            arr[0] = tmp;
        } else {
            // Not changing
        }
    } else {
        // Splitting array in two pieces
        unsigned fLength = arr.size() / 2;

        std::vector <_T> fArr;
        std::vector <_T> sArr;

        for (unsigned i = 0; i < arr.size(); i++) {
            if (i < fLength) {
                fArr.push_back(arr[i]);
            } else {
                sArr.push_back(arr[i]);
            }
        }

        if (maximumThreads <= 0) {
            Mergesort(fArr, 0);
            Mergesort(sArr, 0);
        } else if (maximumThreads == 1) {
            maximumThreads -= 1; // Creating one new thread
            std::thread fThread ([](std::vector <_T> *a, int m = 0){Mergesort(*a, m);}, &fArr, 0);
            Mergesort(sArr, 0);
            fThread.join(); // Waiting for thread to finish
        } else {
            maximumThreads -= 2; // Creating two new threads
            std::thread fThread ([](std::vector <_T> *a, int m = 0){Mergesort(*a, m);}, &fArr, maximumThreads / 2);
            std::thread sThread ([](std::vector <_T> *a, int m = 0){Mergesort(*a, m);}, &sArr, maximumThreads - (maximumThreads / 2));
            fThread.join(); // Waiting for threads to finish
            sThread.join();
        }

        // Arrays sorted. Merging
        unsigned fPos = 0;
        unsigned sPos = 0;
        for (unsigned i = 0; i < arr.size(); i++) {
            if (fPos < fArr.size() && (sPos >= sArr.size() || fArr[fPos] <= sArr[sPos])) {
                arr[i] = fArr[fPos];
                fPos++;
            } else if (sPos < sArr.size()) {
                arr[i] = sArr[sPos];
                sPos++;
            } else {
                // Should not happen
            }
        }
    }
    return;
}
#endif // MERGESORT_THREADS_H_INCLUDED
