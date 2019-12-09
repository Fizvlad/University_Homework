#include "CompMathShared.h"

using namespace std;

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

        string type = reader.Get(solution_name, "type", "");
        double epsilon = reader.GetReal(solution_name, "epsilon", 0.000001);
        string file_name = reader.Get(solution_name, "file", "");

        fstream fs;
        if (file_name != "") {
            fs.open(file_name, fstream::out);
        }

        cout << "\t\tSolution type: " << type << endl;

        if (type == "SimpleIterations") {
            // Simple iterations method
            // Splitting into H and g: Ax=b ---> x=Hx+g
            Matrix h_matrix = identity_matrix(n) - diagonal_matrix(coefficients).inverse() * coefficients;
            Vector g_vector = right_part;
            for (long i = 0; i < n; i++) {
                g_vector(i) /= coefficients(i, i);
            }
            (file_name != "" ? fs : cout) << "\t\tH matrix:" << endl << h_matrix << endl;
            (file_name != "" ? fs : cout) << "\t\tg vector:" << endl << g_vector << endl;
            // Checking if H matrix is correct.
            EigenSolver<Matrix> es(h_matrix); // Will find solution right away;
            (file_name != "" ? fs : cout) << "\t\tEigenvalues of H matrix:" << endl << es.eigenvalues() << endl;
            bool if_stable = true;
            for (long i = 0; i < n; i++) {
                if (abs(es.eigenvalues()(i)) >= 1) {
                    if_stable = false;
                    break;
                }
            }
            (file_name != "" ? fs : cout) << "\t\tApproximation will " << (if_stable ? "" : "not ") << "converge" << endl;
            if (!if_stable) {
                continue;
            }
            // Iterating
            long iteration_num = 0;
            Vector x_curr = zero_vector(n);
            while (true) {
                iteration_num++;
                Vector x_next = h_matrix * x_curr + g_vector;
                bool reached_required_precision = ((x_next - x_curr).norm() < epsilon);
                x_curr = x_next;
                if (reached_required_precision) {
                    break;
                }
            }
            double relative_error = (coefficients * x_curr - right_part).norm() / right_part.norm();
            (file_name != "" ? fs : cout) << "\t\tSolved after " << iteration_num << " iterations." << endl
                                          << "\t\tSolution: " << endl << x_curr << endl
                                          << "\t\tRelative error: " << relative_error << endl;

            cout << "\t\tSolved." << endl;
        } else if (type == "Seidel") {
            // Seidel method
            // Splitting into H and g: Ax=b ---> x=Hx+g
            Matrix ld_inverse = lower_matrix(coefficients, true).inverse();
            Matrix h_matrix = -1 * ld_inverse * upper_matrix(coefficients);
            Vector g_vector = ld_inverse * right_part;
            (file_name != "" ? fs : cout) << "\t\tH matrix:" << endl << h_matrix << endl;
            (file_name != "" ? fs : cout) << "\t\tg vector:" << endl << g_vector << endl;
            // Checking if H matrix is correct.
            EigenSolver<Matrix> es(h_matrix); // Will find solution right away;
            (file_name != "" ? fs : cout) << "\t\tEigenvalues of H matrix:" << endl << es.eigenvalues() << endl;
            bool if_stable = true;
            for (long i = 0; i < n; i++) {
                if (abs(es.eigenvalues()(i)) >= 1) {
                    if_stable = false;
                    break;
                }
            }
            (file_name != "" ? fs : cout) << "\t\tApproximation will " << (if_stable ? "" : "not ") << "converge." << endl;
            if (!if_stable) {
                continue;
            }
            // Iterating
            long iteration_num = 0;
            Vector x_curr = zero_vector(n);
            while (true) {
                iteration_num++;
                Vector x_next = h_matrix * x_curr + g_vector;
                bool reached_required_precision = ((x_next - x_curr).norm() < epsilon);
                x_curr = x_next;
                if (reached_required_precision) {
                    break;
                }
            }
            double relative_error = (coefficients * x_curr - right_part).norm() / right_part.norm();
            (file_name != "" ? fs : cout) << "\t\tSolved after " << iteration_num << " iterations." << endl
                                          << "\t\tSolution: " << endl << x_curr << endl
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
