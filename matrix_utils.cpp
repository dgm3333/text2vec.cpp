#include <vector>
#include <cmath>
#include <algorithm>

using namespace std;

vector<double> colMins(const vector<vector<double>>& x) {
  vector<double> res(x[0].size());
  for(size_t i = 0; i < x[0].size(); i++) {
    double col_min = numeric_limits<double>::max();
    for(const auto& row : x) {
      col_min = min(col_min, row[i]);
    }
    res[i] = col_min;
  }
  return res;
}

vector<double> colMaxs(const vector<vector<double>>& x) {
  vector<double> res(x[0].size());
  for(size_t i = 0; i < x[0].size(); i++) {
    double col_max = numeric_limits<double>::min();
    for(const auto& row : x) {
      col_max = max(col_max, row[i]);
    }
    res[i] = col_max;
  }
  return res;
}

vector<double> rowMins(const vector<vector<double>>& x) {
  vector<double> res(x.size());
  for(size_t i = 0; i < x.size(); i++) {
    res[i] = *min_element(x[i].begin(), x[i].end());
  }
  return res;
}

vector<double> rowMaxs(const vector<vector<double>>& x) {
  vector<double> res(x.size());
  for(size_t i = 0; i < x.size(); i++) {
    res[i] = *max_element(x[i].begin(), x[i].end());
  }
  return res;
}

vector<vector<double>> euclidean_dist(const vector<vector<double>>& x, const vector<vector<double>>& y) {
  if(x.size() != y.size()) {
    throw std::invalid_argument("Matrices should have same number of rows");
  }
  vector<vector<double>> res(x[0].size(), vector<double>(y[0].size(), 0));
  double tmp = 0.0;
  double diff;
  size_t inner_dim = x.size();
  for(size_t i = 0; i < x[0].size(); i++) {
    for(size_t j = 0; j < y[0].size(); j++) {
      tmp = 0.0;
      for(size_t k = 0; k < inner_dim; k++) {
        diff = x[k][i] - y[k][j];
        tmp += diff * diff;
      }
      res[i][j] = sqrt(tmp);
    }
  }
  return res;
}

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

void fill_mat_rand(vector<vector<float>>& mat, size_t ncol) {
  for (auto& row : mat)
    for (size_t j = 0; j < ncol; j++)
      row[j] = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
}

void fill_vec_rand(vector<float>& vec) {
  for(auto& val : vec)
    val = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
}

void fill_vec_val(vector<float>& vec, float val) {
  fill(vec.begin(), vec.end(), val);
}
