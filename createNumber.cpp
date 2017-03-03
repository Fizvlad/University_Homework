#include <iostream>
#include <cstdlib>
#include <ctime>

using namespace std;

char* createNumber(bool ifOwn) { // creating string with number
    cout << "Set number length (higher than 0)(do not count '-'): " << endl;
    unsigned length;
    cin >> length;
    char* in;
    bool sign;
    if(ifOwn) {
        cout << "Please set your number (length: " << length << "): " << endl;

        char first;
        cin >> first;
        if (first == '-') {
            sign = false;
        } else {
            sign = true;
        }

        in = new char[length + !sign + 1];
        in[0] = first;
        for (int i = 1; i < length + !sign; i++) {
            cin >> in[i];
        }
        in[length + !sign] = '\0';
    } else {
        srand(time(0));
        sign = rand() % 2;

        in = new char[length + !sign + 1];
        if (!sign) {
            in[0] = '-';
        }
        for(int i = !sign; i < length + !sign; i++) {
            in[i] = char(48 + (rand() % 10));
        }
        in[length + !sign] = '\0';
    }
    if (in[!sign] == '0' && length > 1) {
        in[!sign] = '1';
    }
    //cout << "Created number : " << in << endl;
    return in;
}
