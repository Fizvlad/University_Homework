#include <iostream>
#include "list.cpp"

using namespace std;

int main()
{
    list a;
    a.push(5);
    a.push(4);
    a.push(1);
    a.push(0);
    a.push(1);
    a.print("a");
    a.sort();
    a.print("a_sorted");
    return 0;
}
