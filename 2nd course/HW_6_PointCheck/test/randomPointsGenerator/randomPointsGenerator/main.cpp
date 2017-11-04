#include <iostream>
#include <fstream>
#include <ctime>
#include <cstdlib>

using namespace std;

int main()
{
    ofstream fout("output.txt");
    long amount = 100000;
    long size = 100;
    srand(time(0));
    fout << amount << endl;
    for (long i = 0; i < amount; i++) {
        fout << (rand() % (2 * size)) - size << " " << (rand() % (2 * size)) - size << endl;
    }
    return 0;
}
