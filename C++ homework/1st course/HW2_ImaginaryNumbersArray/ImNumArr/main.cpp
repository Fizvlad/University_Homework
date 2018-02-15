#include <iostream>
#include "ImNum.h"
#include "ImVector.h"

using namespace std;

void printI(imNumber a, string name) {
    cout << name << ": " << a.getReal();
    float temp = a.getImaginary();
    if (temp > 0) {
        cout << " + " << temp << "i" << endl;
    } else if (temp < 0) {
        cout << " - " << -1 * temp << "i" << endl;
    } else {
        cout << endl;
    }
}
void printI(imNumber a, int name) {
    cout << name << ": " << a.getReal();
    float temp = a.getImaginary();
    if (temp > 0) {
        cout << " + " << temp << "i" << endl;
    } else if (temp < 0) {
        cout << " - " << -1 * temp << "i" << endl;
    } else {
        cout << endl;
    }
}

void printV(imVector v, string name) {
    cout << endl << name << ": " << endl;
    for (unsigned i = 0; i < v.getLength(); i++) {
        printI(v[i], i);
    }
}

int main()
{
    imVector v(1);
    v.resize(2);
    v[0] += 5;
    v[1].set(1, 5);
    v[1] = v[1] + v[0];
    printV(v, "v");

    v.push_back(imNumber(3, 4));
    printV(v, "v");
    v.push_back(v[1] * v[2]);
    printV(v, "v");

    imVector v2(9);
    v2 = v;
    v = v;
    bool a = v == v2;
    cout << "v == v2: " << a << endl;
    v2[v2.getLength() - 1].set(13, 37);
    v[v.getLength() - 1].set(37, 13);
    printV(v, "v");
    printV(v2, "v2");
    a = v != v2;
    cout << "v != v2: " << a << endl;
    return 0;
}
