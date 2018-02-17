#include <iostream>
#include "CompMath.h"

using namespace std;

double task_3_function (double x) {
    return x - (4.0/3.0) * log(x) - (5.0/3.0);
}
double task_3_function_derivative (double x) {
    return x - (4.0/3.0) / x;
}

int main()
{
    Function <Consumer_Function_t> task_3(task_3_function, task_3_function_derivative);
    Interval interval = findDrawdown(task_3, Interval(1e-8, 10));
    cout << interval << endl;
    return 0;
}
