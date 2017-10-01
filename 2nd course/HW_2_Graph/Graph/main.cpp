#include <iostream>
#include "graph.h"

using namespace std;

/*
int f (int &data, unsigned index)
{
    data++;
    cout << "Data #" << index << " increased" << endl;
    return 0;
}

int f2 (int &data, unsigned index)
{
    cout << "Data #" << index << ": " << data << endl;
    return 0;
}
*/

int main()
{
    Graph <int> g;
    /*
        0 - 1 - 2
        |   |   |
        3 - 4 - 5
        |   |   |
        6 - 7 - 8
    */
    g.push(100); // 0
    g.push(101, 1, 0); // 1
    g.push(102, 1, 1); // 2

    g.push(103, 1, 0); // 3
    g.push(104, 2, 1, 3); // 4
    g.push(105, 2, 2, 4); // 5

    g.push(106, 1, 3); // 6
    g.push(107, 2, 4, 6); // 7
    g.push(108, 2, 5, 7); // 8
    g.printInfo();

    cout << "In depth: " << endl;
    g.inDepth(4); // In depth


    cout << "In order: " << endl;
    g.inOrder(); // In order of adding
}
