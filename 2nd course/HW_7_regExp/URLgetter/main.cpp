#include <iostream>
#include <fstream>

using namespace std;

int main()
{
    ifstream fin("input.txt");
    ofstream fout("output.txt");
    fout << "Hello world!" << endl;
    return 0;
}
