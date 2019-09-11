#include <iostream>
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

    double t_0 = reader.GetReal("PROBLEM", "t_0", 0.0);

    Vector y_0(n);
    for (long i = 0; i < n; i++) {
        y_0(i, 0) = reader.GetReal("PROBLEM", cauchy_data_name(i + 1), 0.0);
    }

    cout << "Configuration loaded from " << configuration_path << ":" << endl
         << "Problem:" << endl
         << "\tn = " << n << endl << endl;

    cout << "\tA:" << endl;
    for (long i = 0; i < n; i++) {
        for (long j = 0; j < n; j++) {
            cout << "\t" << coefficients(i, j);
        }
        cout << endl;
    }
    cout << endl;

    cout << "\tt_0 = " << t_0 << endl << endl;

    cout << "\tY_0:" << endl;
    for (long i = 0; i < n; i++) {
        cout << "\t" << y_0(i) << endl;
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

            // Precise solution
            // Getting eigen
            EigenSolver<Matrix> es(coefficients); // Will find solution right away;
            cout << "\t\tEigenvalues:" << endl << es.eigenvalues() << endl;
            cout << "\t\tMatrix of eigenvectors:" << endl << es.eigenvectors() << endl;

            // Getting constants by solving linear system at t=t_0
            Matrix m(n, n);
            for (int i = 0; i < n; i++) {
                for (int j = 0; j < n; j++) {
                    complex<double> temp = es.eigenvectors()(j, i) * exp(es.eigenvalues()(j) * t_0);
                    if (temp.imag() != 0) {
                        cerr << "Attention: For some reason there is non-real value in constants. That must not be this way probably." << endl;
                    }
                    m(i, j) = temp.real();
                }
            }
            Vector constants_vector = m.colPivHouseholderQr().solve(y_0);
            cout << "\t\tConstants vector:" << endl << constants_vector << endl;

            double t = interval_left;
            while (t <= interval_right) {
                Vector value(n);
                for (int i = 0; i < n; i++) {
                    value[i] = 0;
                    for (int j = 0; j < n; j++) {
                        double temp_1 = exp(es.eigenvalues()(j).real() * t);
                        double temp_2 = es.eigenvectors()(j, i).real() * temp_1;
                        double temp_3 = constants_vector(j);
                        //cout << temp_1 << " " << temp_2 << " " << temp_3 << endl;
                        value[i] += temp_3 * temp_2;
                    }
                }
                cout << "\t\tY(" << t << "):" << endl << value << endl;
                t += step;
            }

            cout << "\t\tSolved." << endl;
        } else if (type == "Euler") {

            // TODO


            cout << "\t\tSolved." << endl;
        } else if (type == "Euler_reversed") {
            cerr << "This solution type is currently not supported. Skipping." << endl;
            //cout << "\t\tSolved." << endl;
        } else if (type == "Adams_int_2") {

            // TODO


            cout << "\t\tSolved." << endl;
        } else if (type == "Adams_ext_2") {
            cerr << "This solution type is currently not supported. Skipping." << endl;
            //cout << "\t\tSolved." << endl;
        } else {
            cerr << "\t\tWarning unknown solution type. Skipping." << endl;
        }
    } while (space_position != string::npos);
    return 0;
}
