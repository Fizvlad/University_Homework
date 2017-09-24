#include <iostream>
#include "graph.h"

using namespace std;

int main()
{
    Graph <int> g;
    g.push(101); // 0
    g.push(102, 1, 0); // 1
    g.push(103, 1, 0); // 2
    g.push(104, 1, 1); // 3
    g.push(105, 1, 2); // 4
    g.push(106, 1, 1); // 5
    g.printInfo();

    cout << "In order: " << endl;
    g.inOrder([](int data, unsigned index){ cout << "Data #" << index << ": " << data << endl; }); // In order of adding
    cout << "In depth: " << endl;
    g.inDepth([](int data, unsigned index){ cout << "Data #" << index << ": " << data << endl; }, 5); // In depth
}
