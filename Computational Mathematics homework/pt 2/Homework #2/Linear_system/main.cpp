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

using namespace std;

// Utility functions
string coefficient_name(long i, long j) {
    stringstream t;
    t << "a_" << i << "_" << j;
    return t.str();
}

string component_name(long i) {
    stringstream t;
    t << "b_" << i;
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

    Vector right_part(n);
    for (long i = 0; i < n; i++) {
        right_part(i, 0) = reader.GetReal("PROBLEM", component_name(i + 1), 0.0);
    }

    cout << "Configuration loaded from " << configuration_path << ":" << endl
         << "Problem:" << endl
         << "\tn = " << n << endl << endl;

    cout << "\tA:" << endl << coefficients << endl << endl;

    cout << "\tb = " << right_part << endl << endl;

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
        string file_name = reader.Get(solution_name, "file", "");

        fstream fs;
        if (file_name != "") {
            fs.open(file_name, fstream::out);
        }

        cout << "\t\tSolution type: " << type << endl;

        if (type == "LU") {
            // LU
            // Checking matrix
            if (coefficients.determinant() == 0) {
                cerr << "Provided coefficients matrix is not inversable!" << endl;
                continue;
            }

            // Manually calculating LU composition
            Matrix l_matrix(n, n);
            Matrix u_matrix(n, n);
            for (long i = 0; i < n; i++) {
                for (long j = 0; j < i; j++) {
                    l_matrix(j, i) = 0;
                    u_matrix(i, j) = 0;
                }
                for (long j = i; j < n; j++) {
                    double sum_l = 0;
                    double sum_u = 0;
                    for (long k = 0; k < i - 1; k++) {
                        sum_l += l_matrix(j, k) * u_matrix(k, i);
                        sum_u += l_matrix(i, k) * u_matrix(k, j);
                    }
                    l_matrix(j, i) = coefficients(j, i) - sum_l;
                    u_matrix(i, j) = (coefficients(i, j) - sum_u) / l_matrix(i, i);
                }
            }
            (file_name != "" ? fs : cout) << "\t\tL matrix:" << endl << l_matrix << endl
                                          << "\t\tU matrix:" << endl << u_matrix << endl;

            // Solving Ly = b
            Vector intermidiate(n);
            for (long i = 0; i < n; i++) {
                double sum = 0;
                for (long j = 0; j < i; j++) {
                    sum += l_matrix(i, j) * intermidiate(j);
                }
                intermidiate(i) = (right_part(i) - sum) / l_matrix(i, i);
            }
            (file_name != "" ? fs : cout) << "\t\tSolution for Ly=b:" << endl << intermidiate << endl;

            // Solving Ux = y
            Vector solution(n);
            for (long i = n - 1; i >= 0; i--) {
                double sum = 0;
                for (long j = n - 1; j > i; j--) {
                    sum += u_matrix(i, j) * solution(j);
                }
                solution(i) = intermidiate(i) - sum;
            }
            double relative_error = (coefficients * solution - right_part).norm() / right_part.norm();
            (file_name != "" ? fs : cout) << "\t\tSolution:" << endl << solution << endl
                                          << "\t\tRelative error: " << relative_error << endl;

            cout << "\t\tSolved." << endl;
        } else if (type == "LLT") {
            // LLT
            // Checking matrix
            if (coefficients.transpose() != coefficients) {
                cerr << "Provided coefficients matrix is asymmetric!" << endl;
                continue;
            }
            // TODO: Optionally we can check whether matrix is positive definite

            // Manually calculating LLT composition
            Matrix l_matrix(n, n);
            for (long i = 0; i < n; i++) {
                for (long j = 0; j < i; j++) {
                    l_matrix(j, i) = 0;
                }

                double sum = 0;
                for (long k = 0; k < i; k++) {
                    sum += pow(l_matrix(i, k), 2);
                }
                l_matrix(i, i) = sqrt(coefficients(i, i) - sum);

                for (long j = i + 1; j < n; j++) {
                    double sum = 0;
                    for (long k = 0; k < i; k++) {
                        sum += l_matrix(i, k) * l_matrix(j, k);
                    }
                    l_matrix(j, i) = (coefficients(j, i) - sum) / l_matrix(i, i);
                }
            }
            (file_name != "" ? fs : cout) << "\t\tL matrix:" << endl << l_matrix << endl;

            // Solving Ly = b
            // TODO
            Vector intermidiate(n);
            for (long i = 0; i < n; i++) {
                double sum = 0;
                for (long j = 0; j < i; j++) {
                    sum += l_matrix(i, j) * intermidiate(j);
                }
                intermidiate(i) = (right_part(i) - sum) / l_matrix(i, i);
            }
            (file_name != "" ? fs : cout) << "\t\tSolution for Ly=b:" << endl << intermidiate << endl;

            // Solving L^T * x = y
            Matrix lt_matrix = l_matrix.transpose();
            Vector solution(n);
            for (long i = n - 1; i >= 0; i--) {
                double sum = 0;
                for (long j = n - 1; j >= i; j--) {
                    sum += lt_matrix(i, j) * solution(j);
                }
                solution(i) = (intermidiate(i) - sum) / lt_matrix(i, i);
            }
            double relative_error = (coefficients * solution - right_part).norm() / right_part.norm();
            (file_name != "" ? fs : cout) << "\t\tSolution:" << endl << solution << endl
                                          << "\t\tRelative error: " << relative_error << endl;

            cout << "\t\tSolved." << endl;
        } else if (type == "LIB_EILER") {

            // Correct solution by Eigen lib.
            Vector solution = coefficients.colPivHouseholderQr().solve(right_part);
            double relative_error = (coefficients * solution - right_part).norm() / right_part.norm();
            (file_name != "" ? fs : cout) << "\t\tSolution:" << endl << solution << endl
                                          << "\t\tRelative error: " << relative_error << endl;


            cout << "\t\tSolved." << endl;
        } else {
            cerr << "\t\tWarning unknown solution type. Skipping." << endl;
        }
    } while (space_position != string::npos);
    return 0;
}
