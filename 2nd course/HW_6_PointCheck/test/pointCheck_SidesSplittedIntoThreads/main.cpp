#include <iostream>
#include <fstream>
#include <vector>
#include <utility>
#include "figures.h"
#include <thread>
#include <chrono>

using namespace std;

void threadFunc(Polygon *polygon, vector <Point> *points, short rightSide, size_t sideIndex, vector <bool> *boolArr)
{
    cout << "New thread launched\n";
    for (unsigned i = 0; i < points->size(); i++) {
        Direct d = polygon->getDirect(sideIndex); // Side between sideIndex and sideIndex + 1
        if (points->at(i) == d) {
            // Point lies on Direct containing side. In this situation Point can only be on side or out of borders
            boolArr->at(i) = false;
        } else {
            short pointPosition = -1 + 2 * (points->at(i) > d); // -1 if below, 1 if above
            if (pointPosition != rightSide) {
                boolArr->at(i) = false;
            }
        }
    }
}
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
    vector <short> rightSide(polygon.n(), 0); // Where point must be
    for (size_t i = 0; i < rightSide.size(); i++) {
        rightSide.at(i) = -1 + 2 * (polygon.getPoint((i + 2) % polygon.n()) > polygon.getDirect(i)); // -1 or +1
    }
    // Polygon ready

    vector <bool> boolArr(n, true); // If point is inside of polygon
    chrono::time_point<chrono::steady_clock> start = chrono::steady_clock::now();
    const size_t THREADS_AMOUNT = 5;
    vector <thread> threads(THREADS_AMOUNT);
    for (size_t i = 0; i < THREADS_AMOUNT; i++) {
        threads.at(i) = thread(threadFunc, &polygon, &points, rightSide[i], i, &boolArr);
    }
    for (size_t i = 0; i < THREADS_AMOUNT; i++) {
        threads.at(i).join();
    }
    cout << "Work time: " << chrono::duration_cast <chrono::milliseconds> (chrono::steady_clock::now() - start).count() << "ms" << endl;

    size_t in = 0;
    size_t out = 0;
    for (size_t i = 0; i < boolArr.size(); i++) {
        boolArr.at(i) ? ++in:++out;
    }
    cout << "Points in: " << in << endl;
    cout << "Points out: " << out << endl;
    return 0;
}
