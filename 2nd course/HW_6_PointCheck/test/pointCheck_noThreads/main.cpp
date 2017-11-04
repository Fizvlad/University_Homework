#include <iostream>
#include <fstream>
#include "figures.h"

using namespace std;

int main()
{
    ifstream fin;
    fin.open("../points.txt"); // Points in square -100..100 x -100..100
    size_t n;
    fin >> n;
    cout << "Amount of points: " << n << endl;
    vector <Point> points;
    points.reserve(n);
    for (size_t i = 0; i < n; i++) {
        coord_t x, y;
        fin >> x >> y;
        points.push_back(Point(x, y));
    }
    // Points ready
    Polygon polygon(5, Point(50, -100), Point(100, 20), Point(10, 100), Point(-80, 100), Point(-100, 70));
    // Polygon ready
    size_t in = 0;
    size_t out = 0;
    for (size_t i = 0; i < points.size(); i++) {
        if (polygon.ifPointLiesIn(points.at(i)))
        {
            in++;
        } else {
            out++;
        }
    }
    cout << "Points in: " << in << endl;
    cout << "Points out: " << out << endl;
    return 0;
}
