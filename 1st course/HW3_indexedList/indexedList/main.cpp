#include <iostream>
#include "indexedList.h"

using namespace std;

int main()
{
    FILE* e = freopen("errlog.txt", "a", stderr);
    inList a;
    a.push(123);
    a.print("a");
    a.pushBefore(0, 0);
    a[1] += 198;
    a.print("a");
    inList b(a);
    b.print("b");
    a[0] = -1;
    a.print("a");
    b.print("b");
    b.pushBefore(111,1);
    b.print("b");
    cout << "pop: " << b.pop(1) << endl;
    b.print("b");
    cout << "Trying to pop something out of range" << endl;
    cout << b.pop(99) << endl;
    // Following code won't be executed
    cout << "Trying to access something out of range with []" << endl;
    cout << b[99] << endl;
    fclose(e);
    return 0;
}
