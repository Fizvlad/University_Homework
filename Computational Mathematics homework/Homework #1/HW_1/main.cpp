#include <iostream>
#include "CompMath.h"

using namespace std;

double task_3_function (double x) {
    return x - 4.0 * log(x) / 3.0 - 5.0 / 3.0;
}
double task_3_function_derivative (double x) {
    return 1.0 - 4.0 / (3.0 * x);
}

int main()
{
    Function <Consumer_Function_t> task_3(task_3_function, task_3_function_derivative);
    Point p = findSolution_SimpleIterations(task_3, Interval(1e-10, 10), 1e-10);
    cout.precision(10);
    cout << p << endl;
    return 0;
}
