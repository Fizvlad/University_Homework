#include <iostream>
#include "Nonogram.cpp"
#include <vector>

using namespace std;

int main()
{
    unsigned linesAmount, columnsAmount;

    cin >> linesAmount;
    vector < vector <unsigned> > lines;
    for (unsigned i = 0; i < linesAmount; i++) {
        unsigned groupsAmount;
        cin >> groupsAmount;
        vector <unsigned> groups;
        for (unsigned j = 0; j < groupsAmount; j++) {
            unsigned groupSize;
            cin >> groupSize;
            groups.push_back(groupSize);
        }
        lines.push_back(groups);
    }

    cin >> columnsAmount;
    vector < vector <unsigned> > columns;
    for (unsigned i = 0; i < columnsAmount; i++) {
        unsigned groupsAmount;
        cin >> groupsAmount;
        vector <unsigned> groups;
        for (unsigned j = 0; j < groupsAmount; j++) {
            unsigned groupSize;
            cin >> groupSize;
            groups.push_back(groupSize);
        }
        columns.push_back(groups);
    }

    Nonogram n(lines, columns);

    n.solve();

    n.print(true, true);
    return 0;
}
