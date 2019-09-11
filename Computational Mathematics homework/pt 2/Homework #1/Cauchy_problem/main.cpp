#include <iostream>
#include <sstream>
#include <string>

#include "include/INIReader.h"

using namespace std;

// Utility functions
string coefficient_name(long i, long j) {
    stringstream t;
    t << "a_" << i << "_" << j;
    return t.str();
}

string cauchy_data_name(long i) {
    stringstream t;
    t << "y_0_" << i;
    return t.str();
}


int main(int argc, char **argv)
{
    // Loading configuration file
    string configuration_path;
    if (argc >= 2) {
        configuration_path = string(argv[1]);
    } else {
        cout << "Please enter path to configuration file:" << endl;
        std::getline(cin, configuration_path);
    }

    cout << "Loading configuration from " << configuration_path << endl;
    INIReader reader(configuration_path);

    if (reader.ParseError() != 0) {
        cerr << "Can't load " << configuration_path << endl;
        return 1;
    }

    // Loading configuration. Creating data.
    long n = reader.GetInteger("Problem", "n", 0);
    if (n <= 0) {
        cerr << "n must be >=0" << endl;
        return 1;
    }
    double **coefficients = new double*[n];
    for (long i = 0; i < n; i++) {
        coefficients[i] = new double[n];
        for (long j = 0; j < n; j++) {
            coefficients[i][j] = reader.GetReal("Problem", coefficient_name(i + 1, j + 1), 0.0);
        }
    }

    double t_0 = reader.GetReal("Problem", "t_0", 0.0);

    double *y_0 = new double[n];
    for (long i = 0; i < n; i++) {
        y_0[i] = reader.GetReal("Problem", cauchy_data_name(i + 1), 0.0);
    }
    // TODO: load Approximation parameters.

    cout << "Configuration loaded from " << configuration_path << ":" << endl
         << "Problem:" << endl
         << "\tn = " << n << endl << endl;

    cout << "\tA:" << endl;
    for (long i = 0; i < n; i++) {
        for (long j = 0; j < n; j++) {
            cout << "\t" << coefficients[i][j];
        }
        cout << endl;
    }
    cout << endl;

    cout << "\tt_0 = " << t_0 << endl << endl;

    cout << "\tY_0:" << endl;
    for (long i = 0; i < n; i++) {
        cout << "\t" << y_0[i] << endl;
    }
    cout << endl;

    // Deleting data
    delete[] coefficients;
    delete[] y_0;

    return 0;
}
