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
    long n = reader.GetInteger("PROBLEM", "n", 0);
    if (n <= 0) {
        cerr << "n must be >=0" << endl;
        return 1;
    }
    double **coefficients = new double*[n];
    for (long i = 0; i < n; i++) {
        coefficients[i] = new double[n];
        for (long j = 0; j < n; j++) {
            coefficients[i][j] = reader.GetReal("PROBLEM", coefficient_name(i + 1, j + 1), 0.0);
        }
    }

    double t_0 = reader.GetReal("PROBLEM", "t_0", 0.0);

    double *y_0 = new double[n];
    for (long i = 0; i < n; i++) {
        y_0[i] = reader.GetReal("PROBLEM", cauchy_data_name(i + 1), 0.0);
    }

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

    cout << "Working on solutions:" << endl;
    string solutions = reader.Get("PROBLEM", "solutions", "");

    long space_position_prev;
    long space_position = -1;
    do {
        space_position_prev = space_position;
        space_position = solutions.find(" ", space_position_prev + 1);

        string solution_name;
        if (space_position == string::npos) {
            solution_name = solutions.substr(space_position_prev + 1, solutions.size() - space_position_prev);
        } else {
            solution_name = solutions.substr(space_position_prev + 1, space_position - space_position_prev - 1);
        }

        cout << "\tWorking on \"" << solution_name << "\":" << endl;

        string type = reader.Get(solution_name, "type", "Precise");
        double interval_left = reader.GetReal(solution_name, "interval_left", 0.0);
        double interval_right = reader.GetReal(solution_name, "interval_right", 0.5);
        double step = reader.GetReal(solution_name, "step", 0.1);

        cout << "\t\tSolution type: " << type << endl
             << "\t\tIn [" << interval_left << "; " << interval_right << "] with step " << step << endl;

        if (type == "Precise") {
            // TODO
            cout << "\t\tSolved." << endl;
        } else if (type == "Euler") {
            // TODO
            cout << "\t\tSolved." << endl;
        } else if (type == "Euler_reversed") {
            cerr << "This solution type is currently not supported. Skipping." << endl;
            // TODO
            //cout << "\t\tSolved." << endl;
        } else if (type == "Adams_int_2") {
            // TODO
            cout << "\t\tSolved." << endl;
        } else if (type == "Adams_ext_2") {
            cerr << "This solution type is currently not supported. Skipping." << endl;
            // TODO
            //cout << "\t\tSolved." << endl;
        } else {
            cerr << "\t\tWarning unknown solution type. Skipping." << endl;
        }
    } while (space_position != string::npos);

    // Deleting data
    delete[] coefficients;
    delete[] y_0;

    return 0;
}
