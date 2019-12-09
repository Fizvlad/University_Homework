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

using Eigen::EigenSolver;

std::string coefficient_name(long i, long j, std::string litera = "a") {
    std::stringstream t;
    t << litera << "_" << i << "_" << j;
    return t.str();
}

std::string component_name(long i, std::string litera = "b") {
    std::stringstream t;
    t << litera << "_" << i;
    return t.str();
}

Vector zero_vector(long n) {
    Vector re(n);
    for (long i = 0; i < n; i++) {
        re(i) = 0;
    }
    return re;
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

Matrix identity_matrix(long n) {
    Matrix re(n, n);
    for (long i = 0; i < n; i++) {
        for (long j = 0; j < n; j++) {
            re(i, j) = (i == j ? 1 : 0);
        }
    }
    return re;
}

Matrix diagonal_matrix(Matrix origin) {
    Matrix re = origin;
    long rows = origin.rows();
    long cols = origin.cols();
    for (long i = 0; i < rows; i++) {
        for (long j = 0; j < cols; j++) {
            if (i != j) {
                re(i, j) = 0;
            }
        }
    }
    return re;
}

Matrix lower_matrix(Matrix origin, bool include_diagonal = false) {
    Matrix re = origin;
    long rows = origin.rows();
    long cols = origin.cols();
    for (long i = 0; i < rows; i++) {
        for (long j = 0; j < cols; j++) {
            if (j > i || (!include_diagonal && i == j)) {
                re(i, j) = 0;
            }
        }
    }
    return re;
}

Matrix upper_matrix(Matrix origin, bool include_diagonal = false) {
    Matrix re = origin;
    long rows = origin.rows();
    long cols = origin.cols();
    for (long i = 0; i < rows; i++) {
        for (long j = 0; j < cols; j++) {
            if (j < i || (!include_diagonal && i == j)) {
                re(i, j) = 0;
            }
        }
    }
    return re;
}