#include "CompMathShared.h"
#include <iomanip>

#include "FunctionParser.h"

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
    string p = reader.Get("PROBLEM", "p", "1");
    string q = reader.Get("PROBLEM", "q", "1");
    string r = reader.Get("PROBLEM", "r", "1");
    string f = reader.Get("PROBLEM", "f", "1");

    double x_a = reader.GetReal("PROBLEM", "x_a", 0.0);
    double alpha_1 = reader.GetReal("PROBLEM", "alpha_1", 1.0);
    double alpha_2 = reader.GetReal("PROBLEM", "alpha_2", 0.0);
    double alpha = reader.GetReal("PROBLEM", "alpha", 1.0);

    double x_b = reader.GetReal("PROBLEM", "x_b", 0.0);
    double beta_1 = reader.GetReal("PROBLEM", "beta_1", 1.0);
    double beta_2 = reader.GetReal("PROBLEM", "beta_2", 0.0);
    double beta = reader.GetReal("PROBLEM", "beta", 1.0);

    long n = reader.GetInteger("PROBLEM", "n", 20);

    string file_name = reader.Get("PROBLEM", "file", "");

    fstream fs;
    if (file_name != "") {
        fs.open(file_name, fstream::out);
    }

    cout << "Parsing formulas" << endl;
    FunctionParser p_func(p);
    FunctionParser q_func(q);
    FunctionParser r_func(r);
    FunctionParser f_func(f);

    double h = (x_b - x_a) / n;
    double h_p2 = pow(h, 2);

    cout << "Configuration loaded from " << configuration_path << ":" << endl
         << "Problem:" << endl
         << "\t(" << p << ") * u'' + (" << q << ") * u' + (" << r << ") * u = " << f << endl
         << "\t" << alpha_1 << " * u(" << x_a << ") + " << alpha_2 << " * u'(" << x_a << ") = " << alpha << endl
         << "\t" << beta_1  << " * u(" << x_b << ") + " << beta_2  << " * u'(" << x_a << ") = " << beta  << endl
         << "n = " << n << ". Step: h = " << h << endl << endl;

    (file_name != "" ? fs : cout) << "Values of p | q | r | f" << endl;
    for (long i = 0; i < n + 2; i++) {
        double x_i = x_a - (h / 2) + (h * i);
        (file_name != "" ? fs : cout) << p_func.call(x_i) << " | " << q_func.call(x_i) << " | " << r_func.call(x_i) << " | " << f_func.call(x_i) << endl;
    }
    (file_name != "" ? fs : cout) << endl;

    vector<double> a_coefficients(n + 2);
    vector<double> b_coefficients(n + 2);
    vector<double> c_coefficients(n + 2);
    vector<double> g_coefficients(n + 2);
    a_coefficients[0] = 0;
    b_coefficients[0] = -1 * ((alpha_1 / 2) + (alpha_2 / h));
    c_coefficients[0] = (alpha_1 / 2) - (alpha_2 / h);
    g_coefficients[0] = alpha;
    for (long i = 1; i < n + 1; i++) {
        double x_i = x_a - (h / 2) + (h * i);
        a_coefficients[i] = -1 * ((p_func.call(x_i) / (h_p2)) + (q_func.call(x_i) / (2*h)));
        b_coefficients[i] = -1 * (2 * (p_func.call(x_i) / (h_p2)) + r_func.call(x_i));
        c_coefficients[i] = -1 * ((p_func.call(x_i) / (h_p2)) - (q_func.call(x_i) / (2*h)));
        g_coefficients[i] = f_func.call(x_i);
    }
    a_coefficients[n + 1] = (beta_1 / 2) - (beta_2 / h);
    b_coefficients[n + 1] = -1 * ((beta_1 / 2) + (beta_2 / h));
    c_coefficients[n + 1] = 0;
    g_coefficients[n + 1] = beta;

    // Go-through
    vector<double> s_coefficients(n + 2);
    s_coefficients[0] = c_coefficients[0] / b_coefficients[0];
    vector<double> t_coefficients(n + 2);
    t_coefficients[0] = -1 * g_coefficients[0] / b_coefficients[0];
    for (long i = 1; i < n + 2; i++) {
        s_coefficients[i] = c_coefficients[i] / (b_coefficients[i] - a_coefficients[i] * s_coefficients[i - 1]);
        t_coefficients[i] = (a_coefficients[i] * t_coefficients[i - 1] - g_coefficients[i]) / (b_coefficients[i] - a_coefficients[i] * s_coefficients[i - 1]);
    }

    vector<double> solution(n + 2);
    solution[n + 1] = t_coefficients[n + 1];
    for (long i = n; i >= 0; i--) {
        solution[i] = s_coefficients[i] * solution[i + 1] + t_coefficients[i];
    }

    long width = 12;
    long precision = width - 5;
    (file_name != "" ? fs : cout) << setw(width) << setfill(' ') << "i"
        << setw(width) << setfill(' ') << "x_i"
        << setw(width) << setfill(' ') << "A"
        << setw(width) << setfill(' ') << "B"
        << setw(width) << setfill(' ') << "C"
        << setw(width) << setfill(' ') << "G"
        << setw(width) << setfill(' ') << "s"
        << setw(width) << setfill(' ') << "t"
        << setw(width) << setfill(' ') << "y"
        << endl;
    for (long i = 0; i < n + 2; i++) {
        (file_name != "" ? fs : cout) << setw(width) << setprecision(precision) << setfill(' ') << i
             << setw(width) << setprecision(precision) << setfill(' ') << x_a - (h/2) + (h*i)
             << setw(width) << setprecision(precision) << setfill(' ') << a_coefficients[i]
             << setw(width) << setprecision(precision) << setfill(' ') << b_coefficients[i]
             << setw(width) << setprecision(precision) << setfill(' ') << c_coefficients[i]
             << setw(width) << setprecision(precision) << setfill(' ') << g_coefficients[i]
             << setw(width) << setprecision(precision) << setfill(' ') << s_coefficients[i]
             << setw(width) << setprecision(precision) << setfill(' ') << t_coefficients[i]
             << setw(width) << setprecision(precision) << setfill(' ') << solution[i]
             << endl;
    }
    (file_name != "" ? fs : cout) << endl;

    // Solution with Eiler
    Matrix coefficients(n + 2, n + 2);
    Vector right_part(n + 2);
    for (long i = 0; i < n + 2; i++) {
        for (long j = 0; j < n + 2; j++) {
            if (i == j) {
                coefficients(i, j) = -1 * b_coefficients[i];
            } else if (i == j + 1) {
                coefficients(i, j) = a_coefficients[i];
            } else if (i == j - 1) {
                coefficients(i, j) = c_coefficients[i];
            } else {
                coefficients(i, j) = 0;
            }
        }
        right_part(i) = g_coefficients[i];
    }
    Vector eiler_solution = coefficients.colPivHouseholderQr().solve(right_part);
    (file_name != "" ? fs : cout) << "Eiler Solution:" << endl << eiler_solution << endl << endl;
    cout << "Solved." << endl;
    return 0;
}
