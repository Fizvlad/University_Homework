#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;

string coefficient_name(long i, long j) {
    stringstream t;
    t << "a_" << i << "_" << j;
    return t.str();
}

int main()
{
    cout << "Specify matrix size:" << endl;
    long n;
    cin >> n;

    cout << "Specify output file name:" << endl;
    string file_name;
    cin >> file_name;
    fstream fs;
    fs.open(file_name.c_str(), fstream::out);

    for (long i = 0; i < n; i++) {
        for (long j = 0; j < n; j++) {
            string cn = coefficient_name(i + 1, j + 1);
            fs << cn << "=" << 1.0 / (i + j + 1) << endl;
        }
        fs << endl;
    }
    return 0;
}
