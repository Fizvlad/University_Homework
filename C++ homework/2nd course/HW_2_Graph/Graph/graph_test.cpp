#include <iostream>
#include "graph.h"

using namespace std;

int main()
{
    Graph <int> g;
    cout << g.push(100) << endl; // 0
    cout << g.push(105, 1, 0) << endl; // 1
    cout << g[0] << endl;
    g[0]++;
    cout << g[0] << endl;
    cout << g[1] << endl;
    g[1] += g[0];
    cout << g[1] << endl;

    g.push(110, 1, 0); // 2
    g.push(120, 1, 2); // 3
    g.push(130, 3, 2, 3, 0); // 4
    g.push(130); // 5

    g.printInfo();

    cout << g.pop(5) << endl;

    g.printInfo();

    cout << "Creating copy" << endl;
    Graph <int> g1(g);
    g1.printInfo();
    return 0;
}
