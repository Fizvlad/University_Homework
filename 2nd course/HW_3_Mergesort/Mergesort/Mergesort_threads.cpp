#include <iostream>
#include <vector>
//#include <thread>
//#include <mutex>

const unsigned MERGESORT_MAX_ADDITIONAL_THREADS = 5;

unsigned _Mergesort_openedThreadsAmount = 0;
//std::mutex _Mergesort_mutex;
class _Mergesort_threadsController
{
	unsigned _currentlyOpenedThreadsAmount;
public:
	void addToQueue ()
	{}
	_Mergesort_threadsController (unsigned threadsAmount = 0)
	{}
};

template <typename _T> void Mergesort (std::vector <_T> *arr);
template <typename _T> void Mergesort (std::vector <_T> &arr);

template <typename _T> void Mergesort (std::vector <_T> *arr)
{
    Mergesort(*arr);
    return;
}

template <typename _T> void Mergesort (std::vector <_T> &arr)
{
    // I hope this type have < method
    if (arr.size() == 1) {
        return;
    }
    if (arr.size() == 2) {
        if (arr[1] < arr[0]) {
            int tmp = arr[1];
            arr[1] = arr[0];
            arr[0] = tmp;
        }
        return;
    }

    // Splitting array in two pieces
    unsigned fLength = arr.size() / 2;
    unsigned sLength = arr.size() - fLength;

    std::vector <_T> fArr;
    std::vector <_T> sArr;

    for (unsigned i = 0; i < arr.size(); i++) {
        if (i < fLength) {
            fArr.push_back(arr[i]);
        } else {
            sArr.push_back(arr[i]);
        }
    }

    Mergesort(fArr);
    Mergesort(sArr);

    // Arrays sorted. Merging
    unsigned fPos = 0;
    unsigned sPos = 0;
    for (unsigned i = 0; i < arr.size(); i++) {
        if (sPos >= sLength || fArr[fPos] < sArr[sPos]) {
            arr[i] = fArr[fPos];
            fPos++;
        } else {
            arr[i] = sArr[sPos];
            sPos++;
        }
    }

    return;
}
