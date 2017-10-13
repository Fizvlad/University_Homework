#include <iostream>
#include <fstream>
#include "Nonogram.h"
#include <vector>

using namespace std;

int main()
{
    const bool ifReadFromFile = true;
    const char* filePath = "../Examples/19_58x77.txt";

    // Using ifstream from <fstream>. To use files as input. Can be disabled with ifReadFromFile
    ifstream fin;
    if (ifReadFromFile) {
        fin.open(filePath);
    }

    unsigned linesAmount, columnsAmount;

    (ifReadFromFile?fin:cin) >> linesAmount;
    vector < vector <unsigned> > lines;
    for (unsigned i = 0; i < linesAmount; i++) {
        unsigned groupsAmount;
        (ifReadFromFile?fin:cin) >> groupsAmount;
        vector <unsigned> groups;
        for (unsigned j = 0; j < groupsAmount; j++) {
            unsigned groupSize;
            (ifReadFromFile?fin:cin) >> groupSize;
            groups.push_back(groupSize);
        }
        lines.push_back(groups);
    }

    (ifReadFromFile?fin:cin) >> columnsAmount;
    vector < vector <unsigned> > columns;
    for (unsigned i = 0; i < columnsAmount; i++) {
        unsigned groupsAmount;
        (ifReadFromFile?fin:cin) >> groupsAmount;
        vector <unsigned> groups;
        for (unsigned j = 0; j < groupsAmount; j++) {
            unsigned groupSize;
            (ifReadFromFile?fin:cin) >> groupSize;
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
