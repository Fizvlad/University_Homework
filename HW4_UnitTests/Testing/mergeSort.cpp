using namespace std;

void mergeSort (char** arr, unsigned amount) {
    if (amount == 1) {
    } else if (amount == 2) {
        if (isFirstBigger(arr[0], arr[1])) {
            strSwap(arr, arr + 1);
        }
    } else if (amount > 2) {
        unsigned am1, am2;
        am1 = amount / 2;
        am2 = amount - am1;
        char** arr1 = new char*[am1];
        for (unsigned i = 0; i < am1; i++) {
            arr1[i] = arr[i];
        }
        char** arr2 = new char*[am2];
        for (unsigned i = 0; i < am2; i++) {
            arr2[i] = arr[am1 + i];
        }
        mergeSort(arr1, am1);
        mergeSort(arr2, am2);

        // Merging
        unsigned c1, c2;
        c1 = 0;
        c2 = 0;
        for (unsigned i = 0; i < amount; i++) {
            if (c1 < am1 && (c2 >= am2 || !isFirstBigger(arr1[c1], arr2[c2]))) {
                arr[i] = arr1[c1];
                c1++;
            } else if (c2 < am2 && (c1 >= am1 || isFirstBigger(arr1[c1], arr2[c2]))) {
                arr[i] = arr2[c2];
                c2++;
            }
        }
    }
}
