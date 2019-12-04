#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <math.h>
#include <ctime>

#include "INIReader.h"

#include "Eigen/Dense"
#include <Eigen/Eigenvalues>
typedef Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic> Matrix;
typedef Eigen::Matrix<double, Eigen::Dynamic, 1> Vector;
typedef Eigen::Matrix<std::complex<double>, Eigen::Dynamic, 1> ComplexVector;

using Eigen::EigenSolver;

using namespace std;

// Utility functions
string coefficient_name(long i, long j) {
    stringstream t;
    t << "a_" << i << "_" << j;
    return t.str();
}

Vector random_vector(long n, long maximum = 1000) {
    Vector re(n);
    srand(time(0));
    for (long i = 0; i < n; i++) {
        long sign = (rand() % 2 == 0) ? 1 : -1;
        re(i) = sign * ((rand() % maximum) + (rand() % 1000) / 1000.0);
    }
    return re;
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
        double epsilon = reader.GetReal(solution_name, "epsilon", 0.000001);
        string file_name = reader.Get(solution_name, "file", "");

        fstream fs;
        if (file_name != "") {
            fs.open(file_name, fstream::out);
        }

        cout << "\t\tSolution type: " << type << endl;

        if (type == "PowerIteration") {
            // Power iteration
            // Checking matrix
            if (coefficients.transpose() != coefficients) {
                cerr << "Provided coefficients matrix is asymmetric!" << endl;
                continue;
            }
            // Setting up Y_0
            Vector y_current = random_vector(n);
            (file_name != "" ? fs : cout) << "\t\tY_0:" << endl << y_current << endl;

            // Iterating
            Vector eigenvalue(n);
            long iteration_num = 0;
            long required_precision_reached = -1; // Index of element in which required precision is reached
            while (true) {
                iteration_num++;
                Vector y_next = coefficients * y_current;
                for (long i = 0; i < n; i++) {
                    double old_eigen = eigenvalue(i);
                    eigenvalue(i) = y_next(i) / y_current(i);
                    if (abs(eigenvalue(i) - old_eigen) <= epsilon) {
                        required_precision_reached = i;
                    }
                }
                y_current = y_next;
                if (required_precision_reached != -1) {
                    break;
                }
            }

            (file_name != "" ? fs : cout) << "\t\tSolved after " << iteration_num << " iterations." << endl
                                          << "\t\tMaximum eigenvalue:\n" << eigenvalue(required_precision_reached) << endl;

            cout << "\t\tSolved." << endl;
        } else if (type == "DotProduct") {
            // Dot product
            // Checking matrix
            if (coefficients.transpose() != coefficients) {
                cerr << "Provided coefficients matrix is asymmetric!" << endl;
                continue;
            }
            // Setting up Y_0
            Vector y_current = random_vector(n);
            (file_name != "" ? fs : cout) << "\t\tY_0:" << endl << y_current << endl;

            // Iterating
            double eigenvalue;
            long iteration_num = 0;
            while (true) {
                iteration_num++;
                Vector y_next = coefficients * y_current;
                double old_eigen = eigenvalue;
                eigenvalue = y_next.dot(y_current) / y_current.dot(y_current);
                if (abs(eigenvalue - old_eigen) <= epsilon) {
                    break;
                }
                y_current = y_next;
            }
            (file_name != "" ? fs : cout) << "\t\tSolved after " << iteration_num << " iterations." << endl
                                          << "\t\tMaximum eigenvalue:\n" << eigenvalue << endl;

            cout << "\t\tSolved." << endl;
        } else if (type == "LIB_EILER") {
            // Correct solution by Eigen lib.
            EigenSolver<Matrix> es(coefficients); // Will find solution right away;
            ComplexVector eigenvalues = es.eigenvalues();
            (file_name != "" ? fs : cout) << "\t\tEigenvalues:" << endl << eigenvalues << endl;
            complex<double> max_eigenvalue = eigenvalues(0);
            for (long i = 1; i < n; i++) {
                if (abs(eigenvalues(i)) > abs(max_eigenvalue)) {
                    max_eigenvalue = eigenvalues(i);
                }
            }
            (file_name != "" ? fs : cout) << "\t\tMaximum of eigenvalues: " << max_eigenvalue << endl;

            cout << "\t\tSolved." << endl;
        } else {
            cerr << "\t\tWarning unknown solution type. Skipping." << endl;
        }
    } while (space_position != string::npos);
    return 0;
}
