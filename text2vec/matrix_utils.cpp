#include <vector>
#include <cmath>
#include <algorithm>
#include <iostream>
#include <cstdlib>

using namespace std;

// Function to compute column minimums of a NumericMatrix
std::vector<double> colMins(const std::vector<std::vector<double>>& x) {
    std::vector<double> res(x[0].size(), std::numeric_limits<double>::max());

    for (const auto& row : x) {
        for (size_t j = 0; j < row.size(); j++) {
            res[j] = std::min(res[j], row[j]);
        }
    }

    return res;
}

// Function to compute column maximums of a NumericMatrix
std::vector<double> colMaxs(const std::vector<std::vector<double>>& x) {
    std::vector<double> res(x[0].size(), std::numeric_limits<double>::lowest());

    for (const auto& row : x) {
        for (size_t j = 0; j < row.size(); j++) {
            res[j] = std::max(res[j], row[j]);
        }
    }

    return res;
}

// Function to compute row minimums of a NumericMatrix
std::vector<double> rowMins(const std::vector<std::vector<double>>& x) {
    std::vector<double> res(x.size(), std::numeric_limits<double>::max());

    for (size_t i = 0; i < x.size(); i++) {
        for (const auto& val : x[i]) {
            res[i] = std::min(res[i], val);
        }
    }

    return res;
}

// Function to compute row maximums of a NumericMatrix
std::vector<double> rowMaxs(const std::vector<std::vector<double>>& x) {
    std::vector<double> res(x.size(), std::numeric_limits<double>::lowest());

    for (size_t i = 0; i < x.size(); i++) {
        for (const auto& val : x[i]) {
            res[i] = std::max(res[i], val);
        }
    }

    return res;
}


// Function to compute the Euclidean distance between two NumericMatrices
std::vector<std::vector<double>> euclidean_dist(const std::vector<std::vector<double>>& x, const std::vector<std::vector<double>>& y) {
    if (x.size() != y.size()) {
        throw std::runtime_error("Matrices should have the same number of rows");
    }

    std::vector<std::vector<double>> res(x[0].size(), std::vector<double>(y[0].size(), 0.0));

    for (size_t i = 0; i < x[0].size(); i++) {
        for (size_t j = 0; j < y[0].size(); j++) {
            double tmp = 0.0;
            for (size_t k = 0; k < x.size(); k++) {
                double diff = x[k][i] - y[k][j];
                tmp += diff * diff;
            }
            res[i][j] = std::sqrt(tmp);
        }
    }

    return res;
}


// Function to convert a vector of vectors to a NumericMatrix
vector<vector<double>> convert2Rmat(vector<vector<float>>& mat, size_t ncol) {

    vector<vector<double>> res(mat.size(), vector<double>(ncol));

    for (size_t i = 0; i < mat.size(); i++)
        for (size_t j = 0; j < ncol; j++)
            res[i][j] = mat[i][j];
    return res;

}

void fill_mat_val(vector<vector<float>>& mat, size_t ncol, float val) {
    for (auto& row : mat)
        fill(row.begin(), row.begin() + ncol, val);
}


// Function to fill a matrix with random values between runif_min and runif_max
void fill_mat_rand(std::vector<std::vector<float>>& mat, size_t ncol, float runif_min, float runif_max) {
    for (auto& row : mat) {
        for (size_t j = 0; j < ncol; j++) {
            row[j] = runif_min + static_cast<float>(std::rand()) / (static_cast<float>(RAND_MAX / (runif_max - runif_min)));
        }
    }
}


// Function to fill a vector with random values between runif_min and runif_max
void fill_vec_rand(std::vector<float>& vec, float runif_min, float runif_max) {
    for (auto& val : vec)
        val = runif_min + static_cast<float>(std::rand()) / (static_cast<float>(RAND_MAX / (runif_max - runif_min)));
}

// Function to fill a vector with a specific value
void fill_vec_val(vector<float>& vec, float val) {
    fill(vec.begin(), vec.end(), val);
}

