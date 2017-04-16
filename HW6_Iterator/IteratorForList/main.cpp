#include <iostream>
#include "list.h"

using namespace std;

int main()
{
    list <const char*> l1;
    l1.push("first");
    l1.push("third");
    l1.pushBefore("second", 1);
    listIterator<const char*> a = l1.begin();
    cout << l1.pop(2) << endl; // third
    cout << l1.pop(1) << endl; // second
    cout << *l1.begin() << endl; // first
    cout << (l1.begin() == l1.end()) << endl; // 1
    l1.pushAfter("must be last", 0);
    cout << *l1.end() << endl; // must be last
    cout << (l1.begin() != l1.end()) << endl; // 1
    cout << *--l1.end() << endl; // first
    return 0;
}
