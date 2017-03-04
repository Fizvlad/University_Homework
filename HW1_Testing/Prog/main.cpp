#include <cstdio>
#include <iostream>
#include "createNumber.h"
#include "longNumbers.h"

using namespace std;

int main() {
    long_number example1, example2;

    char* in = createNumber(true);
    example1.set(in);
    delete in;
    in = createNumber(true);
    example2.set(in);
    delete in;
    example1.add(&example2);

    printf("%s%c", example1.print(), '\n');

    example1.multiple(&example2);

    printf("%s%c", example1.print(), '\n');

    return 0;
}
