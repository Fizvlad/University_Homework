#include <iostream>
#include <fstream>
#include "Nonogram.cpp"
#include <vector>

using namespace std;

int main()
{
    const char* filePath = "../Examples/13_40x35.txt";
    ifstream fin(filePath);

    unsigned linesAmount, columnsAmount;

    fin >> linesAmount;
    vector < vector <unsigned> > lines;
    for (unsigned i = 0; i < linesAmount; i++) {
        unsigned groupsAmount;
        fin >> groupsAmount;
        vector <unsigned> groups;
        for (unsigned j = 0; j < groupsAmount; j++) {
            unsigned groupSize;
            fin >> groupSize;
            groups.push_back(groupSize);
        }
        lines.push_back(groups);
    }

    fin >> columnsAmount;
    vector < vector <unsigned> > columns;
    for (unsigned i = 0; i < columnsAmount; i++) {
        unsigned groupsAmount;
        fin >> groupsAmount;
        vector <unsigned> groups;
        for (unsigned j = 0; j < groupsAmount; j++) {
            unsigned groupSize;
            fin >> groupSize;
            groups.push_back(groupSize);
        }
        columns.push_back(groups);
    }

    Nonogram n(lines, columns);

    n.solve();

    n.print();

    fin.close();
    return 0;
}
