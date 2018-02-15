#include <iostream>
#include "createNumber.h"
#include "longNumbers.h"

using namespace std;

int main() {
    long_number example1, example2;

    cout << "First number : " << endl;
    char* in = createNumber(true);
    cout << "Created number : " << in << endl;
    example1.set(in);
    cout << "long_number : " << example1.print() << endl;

    cout << endl << "Second number : " << endl;
    in = createNumber(true);
    cout << "Created number : " << in << endl;
    example2.set(in);
    cout << "long_number : " << example2.print() << endl;

    cout << endl << "Adding number #2" << endl;
    example1.add(&example2);
    cout << "Result : " << example1.print() << endl;

    cout << endl << "Substracting number #2" << endl;
    example1.substract(&example2);
    cout << "Result : " << example1.print() << endl;

    cout << endl << "Multiplying by number #2" << endl;
    example1.multiple(&example2);
    cout << "Result : " << example1.print() << endl;
    return 0;
}
