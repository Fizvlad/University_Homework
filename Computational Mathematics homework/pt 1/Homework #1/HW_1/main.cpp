#include <iostream>
#include "CompMath.h"

using namespace std;

double task_3_function (double x) {
    return x - 4.0 * log(x) / 3.0 - 5.0 / 3.0;
}
double task_3_function_derivative (double x) {
    return 1.0 - 4.0 / (3.0 * x);
}

double task_2_function_1 (double x, double y) {
    return pow(x, 3) - 0.5 * y + 0.023 * (pow(x, 2) + pow(y, 2)) + 0.016;
}
double task_2_function_1_x_derivative (double x, double y) {
    return 3 * pow(x, 2) + 0.046 * x;
}
double task_2_function_1_y_derivative (double x, double y) {
    return -0.5 + 0.046 * y;
}
double task_2_function_2 (double x, double y) {
    return 0.121 * pow(y, 2) + 1.82 * y - 0.06 * x * y - 0.024;
}
double task_2_function_2_x_derivative (double x, double y) {
    return -0.06 * y;
}
double task_2_function_2_y_derivative (double x, double y) {
    return 0.242 * y + 1.82 - 0.06 * x;
}

int main()
{
    cout << "x - (4/3) * ln(x) = 5/3" << endl;
    Function <Consumer_Function_t> task_3(task_3_function, task_3_function_derivative);
    cout.precision(10);
    cout << "Solution with iterations: x = " << findSolution_SimpleIterations(task_3, Interval(1e-10, 10), 1e-10) << endl;
    cout << "Solution with Newton: x = " << findSolution_Newton(task_3, Interval(1e-10, 10), 1e-10) << endl;

    cout << endl;

    cout << "System: " << endl << "x^3 - 0.5y + 0.023(x^2 + y^2) + 0.016 = 0" << endl << "0.121y^2 + 1.82y - 0.06xy - 0.024 = 0" << endl;
    BiFunctions <Biconsumer_Function_t> task_2(task_2_function_1, task_2_function_1_x_derivative, task_2_function_1_y_derivative,
                                               task_2_function_2, task_2_function_2_x_derivative, task_2_function_2_y_derivative);
    pair<Point, Point> p = findSolution_Newton(task_2, -2.0, -16.0, 1e-10);
    cout << "Solution with Newton: x = " << p.first << "; y = " << p.second << endl;
    return 0;
}
