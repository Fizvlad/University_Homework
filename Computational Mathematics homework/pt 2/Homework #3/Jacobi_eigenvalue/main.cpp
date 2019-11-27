#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <math.h>

#include "INIReader.h"

#include "Eigen/Dense"
#include <Eigen/Eigenvalues>
typedef Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic> Matrix;
typedef Eigen::Matrix<double, Eigen::Dynamic, 1> Vector;

using Eigen::EigenSolver;

using namespace std;

// Utility functions
string coefficient_name(long i, long j) {
    stringstream t;
    t << "a_" << i << "_" << j;
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
    const long n = reader.GetInteger("PROBLEM", "n", 0);
    if (n <= 0) {
        cerr << "n must be >=0" << endl;
        return 1;
    }

    Matrix coefficients(n, n);
    for (long i = 0; i < n; i++) {
        for (long j = 0; j < n; j++) {
            coefficients(i, j) = reader.GetReal("PROBLEM", coefficient_name(i + 1, j + 1), 0.0);
        }
    }

    cout << "Configuration loaded from " << configuration_path << ":" << endl
         << "Problem:" << endl
         << "\tn = " << n << endl << endl;

    cout << "\tA:" << endl << coefficients << endl << endl;

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

        string type = reader.Get(solution_name, "type", "");
        double epsilon = reader.GetReal(solution_name, "epsilon", 0.001);
        string file_name = reader.Get(solution_name, "file", "");

        fstream fs;
        if (file_name != "") {
            fs.open(file_name, fstream::out);
        }

        cout << "\t\tSolution type: " << type << endl;

        if (type == "Jacobi") {
            // Jacobi
            // Checking matrix
            if (coefficients.transpose() != coefficients) {
                cerr << "Provided coefficients matrix is asymmetric!" << endl;
                continue;
            }
            // Iterating
            Matrix coefficients_k = coefficients;
            unsigned iteration_num = 0;
            while (true) {
                // Looking for largest element
                double max_val = abs(coefficients_k(0, 1));
                long max_i = 0;
                long max_j = 1;
                for (long i = 0; i < n; i++) {
                    for (long j = i + 1; j < n; j++) {
                        double val = abs(coefficients_k(i, j));
                        if (val > max_val) {
                            max_val = val;
                            max_i = i;
                            max_j = j;
                        }
                    }
                }
                if (max_val < epsilon) {
                    break;
                }
                // Getting rotation angle
                double phi = 0.5 * atan(2 * coefficients_k(max_i, max_j) / (coefficients_k(max_i, max_i) - coefficients_k(max_j, max_j)));
                // Building rotation matrix
                Matrix r_m(n, n);
                for (long i = 0; i < n; i++) {
                    for (long j = 0; j < n; j++) {
                        r_m(i, j) = 0;
                    }
                }
                for (long i = 0; i < n; i++) {
                    r_m(i, i) = 1;
                }
                r_m(max_i, max_i) = cos(phi);
                r_m(max_j, max_i) = sin(phi);
                r_m(max_j, max_j) = cos(phi);
                r_m(max_i, max_j) = -sin(phi);
                (file_name != "" ? fs : cout) << "\t\tRotation #" << iteration_num + 1 << "(" << max_i + 1 << "; " << max_j + 1 << ")" << " at angle " << phi << ":" << endl << r_m << endl;
                // Rotating
                coefficients_k = r_m.transpose() * coefficients_k * r_m;

                (file_name != "" ? fs : cout) << "\t\tNew matrix:" << endl << coefficients_k << endl << endl;

                iteration_num++;
            }

            (file_name != "" ? fs : cout) << "\t\tSolved after " << iteration_num << " iterations." << endl
                                          << "\t\tEigenvalues:" << endl;
            for (long i = 0; i < n; i++) {
                (file_name != "" ? fs : cout) << coefficients_k(i, i) << endl;
            }

            cout << "\t\tSolved." << endl;
        } else if (type == "LIB_EILER") {
            // Correct solution by Eigen lib.
            EigenSolver<Matrix> es(coefficients); // Will find solution right away;
            (file_name != "" ? fs : cout) << "\t\tEigenvalues:" << endl << es.eigenvalues() << endl;


            cout << "\t\tSolved." << endl;
        } else {
            cerr << "\t\tWarning unknown solution type. Skipping." << endl;
        }
    } while (space_position != string::npos);
    return 0;
}
