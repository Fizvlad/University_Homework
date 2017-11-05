#include <iostream>
#include <fstream>
#include <vector>
#include <utility>
#include "figures.h"
#include <atomic>
#include <thread>
#include <chrono>

using namespace std;

void threadFunc(Polygon *polygon, vector <Point> *points, atomic_int *in, atomic_int *out)
{
    cout << "New thread launched\n";
    for (unsigned i = 0; i < points.size(); i++) {
        if (points->at(i) > polygon->getDirect(sideIndex)) {
            (*in)++;
        } else {
            (*out)++;
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
    vector <short> rightSide(polygon.n());
    for (size_t i = 0; i < rightSide.size(); i++) {
        rightSide.at(i) = -1 + 2 * (polygon.getPoint((i + 2) % polygon.n()) > polygon.getDirect(i));
    }
    // Polygon ready

    chrono::time_point<chrono::steady_clock> start = chrono::steady_clock::now();
    atomic_int in(0);
    atomic_int out(0);
    const size_t THREADS_AMOUNT = 5;
    vector <thread> threads(THREADS_AMOUNT);
    for (size_t i = 0; i < THREADS_AMOUNT; i++) {
        threads.at(i) = thread(threadFunc, &polygon, &points, &in, &out);
    }
    for (size_t i = 0; i < THREADS_AMOUNT; i++) {
        threads.at(i).join();
    }
    cout << "Points in: " << in << endl;
    cout << "Points out: " << out << endl;
    cout << "Work time: " << chrono::duration_cast <chrono::milliseconds> (chrono::steady_clock::now() - start).count() << "ms" << endl;
    return 0;
}
