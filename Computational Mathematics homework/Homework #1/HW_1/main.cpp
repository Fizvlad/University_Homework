#include <iostream>
#include "CompMath.h"

using namespace std;

double task_1_function (double x) {
    return std::tan(x) - std::pow(std::exp(1), std::pow(x, 2));
}

double task_3_function (double x) {
    return x - (4.0/3.0) * log(x) - (5.0/3.0);
}

int main()
{
    cout << findDrawdown(task_3_function, 1e-8, 10, 3) << endl;
    return 0;
}
