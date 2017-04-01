#include <iostream>
#include <cstdlib>
#include <ctime>
#include "mergeSort.h"

using namespace std;

int main()
{
    unsigned short amount;
    cout << "Set string amount: " << endl;
    cin >> amount;

    unsigned short l;
    cout << "Set string length (less than 21 symbol): " << endl;
    cin >> l;

    if (l == 0 || l > 20) {
        cout << "Wrong length!" << endl;
        return 0;
    }


    char** arr = new char*[amount];
    srand(time(0));
    for (int i = 0; i < amount; i++) {
        arr[i] = new char[l + 1];
        for (int j = 0; j < l; j++) {
            arr[i][j] = rand() % 26 + 65;
        }
        arr[i][l] = '\0';
    }

    cout << "Array: " << endl;
    for (int i = 0; i < amount; i++) {
        cout << arr[i] << endl;
    }

    mergeSort(arr, amount);
    cout << "Sorted!" << endl;

    cout << "Sorted array: " << endl;
    for (int i = 0; i < amount; i++) {
        cout << arr[i] << endl;
    }
}
