#include <iostream>
#include "mergeSort.h"

using namespace std;

int main()
{
    char** str = new char*[2];

    unsigned length1;
    cin >> length1;
    str[0] = new char[length1 + 1];
    cin >> str[0];
    str[0][length1] = '\0';

    unsigned length2;
    cin >> length2;
    str[1] = new char[length2 + 1];
    cin >> str[1];
    str[1][length2] = '\0';

    if (isFirstBigger(str[0], str[1])) {
        cout << "1\n";
    } else {
        cout << "0\n";
    }
    return 0;
}
