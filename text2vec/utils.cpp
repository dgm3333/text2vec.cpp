// utils.cpp
// Copyright (C) 2015 - 2016	Dmitriy Selivanov
// This file is part of text2vec
//
// text2vec is free software: you can redistribute it and/or modify it
// under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 2 of the License, or
// (at your option) any later version.
//
// text2vec is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.	See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with text2vec.	If not, see <http://www.gnu.org/licenses/>.

#include <chrono>
#include <format>
#include <locale>

#include "text2vec.h"
using namespace std;

// fast integer hashing
uint32_t fast_int_hash(uint32_t a) {
	a = ((a >> 16) ^ a) * 0x45d9f3b;
	a = ((a >> 16) ^ a) * 0x45d9f3b;
	a = ((a >> 16) ^ a);
	return a;
}


// Get current date/time, format is YYYY-MM-DD HH:mm:ss
const std::string currentDateTime() {
	auto now = std::chrono::system_clock::now();
	auto now_t = std::chrono::zoned_time(std::chrono::current_zone(), now);

	return std::format("{:%Y-%m-%d %T}", now_t);
}


std::vector<std::string> stringVectorToStdVector(std::vector<std::string> terms_raw) {

	std::vector<std::string> result;
	result.reserve(terms_raw.size());

	for (auto& it: terms_raw) {
		result.push_back(it);
	}

	return(result);
}

std::vector<std::string> generate_ngrams(const std::vector< std::string> &terms,
										 const uint32_t ngram_min,
										 const uint32_t ngram_max,
										 std::unordered_set<std::string> &stopwords,
										 const string ngram_delim) {
	uint32_t len = (uint32_t)terms.size();
	vector<string> ngrams;
	vector<string> terms_filtered;
	terms_filtered.reserve(len);
	ngrams.reserve(len);

	for (auto& it: terms) {
		if(stopwords.find(it) == stopwords.end())
			terms_filtered.push_back(it);
	}

	if( ngram_min == ngram_max &&	ngram_max == 1 ) {
		return(terms_filtered);
	}

	string k_gram;
	size_t k, j_max_observed;

	for(size_t j = 0; j < len; j ++ ) {
		k = 1;
		j_max_observed = j;
		while (k <= ngram_max && j_max_observed < terms_filtered.size()) {

			if( k == 1) {
				k_gram = terms_filtered[j_max_observed];
			} else
				k_gram = k_gram + ngram_delim + terms_filtered[j_max_observed];

			if(k >= ngram_min) {
				ngrams.push_back(k_gram);
			}
			j_max_observed = j + k;
			k = k + 1;
		}
	}
	return(ngrams);
}

std::vector<std::string> fixed_char_tokenizer(const std::string &s, char delim) {

	std::stringstream ss(s);
	std::string item;
	std::vector<std::string> elems;

	while (std::getline(ss, item, delim)) {
		elems.push_back(item);
		//elems.push_back(std::move(item));
	}

	return elems;
}

std::vector<std::vector < std::string> > cpp_fixed_char_tokenizer(std::vector<std::string> x, char delim) {

	std::vector<std::string> docs = stringVectorToStdVector(x);
	std::vector<std::vector < std::string> > tokens;
	tokens.reserve(x.size());

	for(auto& doc : docs) {
		tokens.push_back(fixed_char_tokenizer(doc, delim));
	}

	return tokens;
}




#include <vector>
#include <cmath>
#include <algorithm>
#include <iostream>




// From original R
// @name split_vector
// @title Generating indexes for splitting vector into chunks
// @description Generating indexes for splitting vector into chunks for parallel processing.
// @details Parameters granularity and splits controls the numer of chunks in returned list.
//       Number of chunks in resulted list in general is equal granularity * splits
// @param vec \link{list} or \link{vector} to split
// @param granularity \link{integer} -  granularity is useful for management of granularity
//      of splits.If you expect that computational time on each chunk of your data will
//      be distributed nerarly uniformly, granularity = 1 is good choice because of little overheads
//      in syncronizing parallel processes.
// @param splits \link{integer} - controls number of parallel jobs you have planned.
//      Usually should be equal to number of cores in the machine.
// @return \link{list} each element is a \link{integer} \link{vector} pair.
//      First element in pair is lower index, second element is upper index.

// Function to split a vector into chunks for parallel processing
std::vector<std::vector<int>> split_vector(const std::vector<int>& vec, int splits, int granularity) {
    std::vector<std::vector<int>> result;
    if (vec.size() < splits * granularity) {
        std::cout << "Length of input is too small for splitting for a given number of splits and level of parallelism. Assuming no splits." << std::endl;
        result.push_back(vec);
        return result;
    }

    int chunkSize = vec.size() / (splits * granularity);
    int remainder = vec.size() % (splits * granularity);

    int startIndex = 0;
    for (int i = 0; i < splits * granularity; i++) {
        int endIndex = startIndex + chunkSize;
        if (i < remainder) endIndex++;

        result.push_back(std::vector<int>(vec.begin() + startIndex, vec.begin() + endIndex));
        startIndex = endIndex;
    }

    return result;
}
std::vector<std::vector<double>> split_vector(const std::vector<double>& vec, int splits, int granularity) {
    std::vector<std::vector<double>> result;
    if (vec.size() < splits * granularity) {
        std::cout << "Length of input is too small for splitting for a given number of splits and level of parallelism. Assuming no splits." << std::endl;
        result.push_back(vec);
        return result;
    }

    int chunkSize = vec.size() / (splits * granularity);
    int remainder = vec.size() % (splits * granularity);

    int startIndex = 0;
    for (int i = 0; i < splits * granularity; i++) {
        int endIndex = startIndex + chunkSize;
        if (i < remainder) endIndex++;

        result.push_back(std::vector<double>(vec.begin() + startIndex, vec.begin() + endIndex));
        startIndex = endIndex;
    }

    return result;
}
std::vector<std::string> split_vector(const std::string& str, int splits, int granularity) {
    std::vector<std::string> result;
    if (str.size() < splits * granularity) {
        std::cout << "Length of input is too small for splitting for a given number of splits and level of parallelism. Assuming no splits." << std::endl;
        result.push_back(str);
        return result;
    }

    int chunkSize = str.size() / (splits * granularity);
    int remainder = str.size() % (splits * granularity);

    int startIndex = 0;
    for (int i = 0; i < splits * granularity; i++) {
        int endIndex = startIndex + chunkSize;
        if (i < remainder) endIndex++;

        result.push_back(std::string(str.begin() + startIndex, str.begin() + endIndex));
        startIndex = endIndex;
    }

    return result;
}

// From original R
// @name split_into
// @title Split a vector for parallel processing
// @description This function splits a vector into \code{n} parts of roughly
//   equal size. These splits can be used for parallel processing. In general,
//   \code{n} should be equal to the number of jobs you want to run, which
//   should be the number of cores you want to use.
// @param vec input vector
// @param n \code{integer} desired number of chunks
// @return \code{list} with \code{n} elements, each of roughly equal length
// @export


// Function to split a vector into n parts of roughly equal size
std::vector<std::vector<int>> split_into(const std::vector<int>& vec, int n) {
    std::vector<std::vector<int>> result;
    int chunkSize = vec.size() / n;
    int remainder = vec.size() % n;

    int startIndex = 0;
    for (int i = 0; i < n; i++) {
        int endIndex = startIndex + chunkSize;
        if (i < remainder) endIndex++;

        result.push_back(std::vector<int>(vec.begin() + startIndex, vec.begin() + endIndex));
        startIndex = endIndex;
    }

    return result;
}
std::vector<std::vector<double>> split_into(const std::vector<double>& vec, int n) {
    std::vector<std::vector<double>> result;
    int chunkSize = vec.size() / n;
    int remainder = vec.size() % n;

    int startIndex = 0;
    for (int i = 0; i < n; i++) {
        int endIndex = startIndex + chunkSize;
        if (i < remainder) endIndex++;

        result.push_back(std::vector<double>(vec.begin() + startIndex, vec.begin() + endIndex));
        startIndex = endIndex;
    }

    return result;
}
std::vector<std::string> split_into(const std::string& str, int n) {
    std::vector<std::string> result;
    int chunkSize = str.size() / n;
    int remainder = str.size() % n;

    int startIndex = 0;
    for (int i = 0; i < n; i++) {
        int endIndex = startIndex + chunkSize;
        if (i < remainder) endIndex++;

        result.push_back(std::string(str.begin() + startIndex, str.begin() + endIndex));
        startIndex = endIndex;
    }

    return result;
}
std::vector<std::vector<std::string>> split_into(const std::vector<std::string>& vec, int n) {
    std::vector<std::vector<std::string>> result;
    int chunkSize = vec.size() / n;
    int remainder = vec.size() % n;

    int startIndex = 0;
    for (int i = 0; i < n; i++) {
        int endIndex = startIndex + chunkSize;
        if (i < remainder) endIndex++;

        result.push_back(std::vector<std::string>(vec.begin() + startIndex, vec.begin() + endIndex));
        startIndex = endIndex;
    }

    return result;
}

// From original R
// @details This is the natural log of the discrete binomial probability mass function scaled
// by the inverse binomial coefficient with special care taken to avoid negative infinity
// resulting from log(0). A slightly slower but more intuitive way of writing this would be
// \code{function(k, n, p) {
//   out <- dbinom(k, n, p, log = TRUE) - log(choose(n, k))
//   replace(out, out == -Inf, 0)
// }}
// This is used to create a log-likelihood ratio with 1 degree of freedom for bi-gram analysis

// Function to calculate the log of the discrete binomial probability mass function
double L_func(double p, int n, int k) {
    return k * log(p + (p == 0)) + (n - k) * log(1 - p + (1 - p == 0));
}


// From original R
// (numerically robust) Dimension reduction via Jensen-Shannon Divergence & Principal Components
// 
// This function is largely a copy of the repsective function in
// https://github.com/cpsievert/LDAvis/blob/master/R/createJSON.R, however,
// with a fix to avoid log(0) proposed by Maren-Eckhoff in
// https://github.com/cpsievert/LDAvis/issues/56
// 
// @param phi matrix, with each row containing the distribution over terms
// for a topic, with as many rows as there are topics in the model, and as
// many columns as there are terms in the vocabulary.

// Function to perform dimension reduction via Jensen-Shannon Divergence and Principal Components
std::vector<std::vector<double>> jsPCA_robust(const std::vector<std::vector<double>>& phi) {
    int K = phi.size();
    int V = phi[0].size();

    // Compute pairwise distance between topic distributions using Jensen-Shannon divergence
    std::vector<std::vector<double>> dist_mat(K, std::vector<double>(K, 0.0));
    for (int i = 0; i < K; i++) {
        for (int j = i + 1; j < K; j++) {
            double m = 0.0;
            for (int v = 0; v < V; v++) {
                m += 0.5 * (phi[i][v] + phi[j][v]);
            }
            double kl_div = 0.0;
            for (int v = 0; v < V; v++) {
                if (phi[i][v] > 0) kl_div += 0.5 * phi[i][v] * log(phi[i][v] / m);
                if (phi[j][v] > 0) kl_div += 0.5 * phi[j][v] * log(phi[j][v] / m);
            }
            dist_mat[i][j] = dist_mat[j][i] = kl_div;
        }
    }

    // Reduce the K by K proximity matrix down to K by 2 using PCA
    std::vector<std::vector<double>> result;
    std::vector<double> eigen_values(K, 0.0);
    std::vector<std::vector<double>> eigen_vectors(K, std::vector<double>(K, 0.0));

    // Perform PCA
    for (int i = 0; i < K; i++) {
        double sum = 0.0;
        for (int j = 0; j < K; j++) {
            sum += dist_mat[i][j];
        }
        eigen_values[i] = sum / K;
    }

    for (int i = 0; i < K; i++) {
        for (int j = 0; j < K; j++) {
            dist_mat[i][j] -= eigen_values[i] + eigen_values[j];
        }
    }

    // Eigen decomposition
    for (int i = 0; i < K; i++) {
        eigen_vectors[i][i] = 1.0;
        for (int j = 0; j < i; j++) {
            double dot_product = 0.0;
            for (int v = 0; v < V; v++) {
                dot_product += phi[i][v] * phi[j][v];
            }
            for (int k = 0; k < K; k++) {
                eigen_vectors[i][k] -= 2 * dot_product * eigen_vectors[j][k];
            }
        }
    }

    // Normalize eigen vectors
    for (int i = 0; i < K; i++) {
        double length = 0.0;
        for (int j = 0; j < K; j++) {
            length += eigen_vectors[i][j] * eigen_vectors[i][j];
        }
        length = sqrt(length);
        for (int j = 0; j < K; j++) {
            eigen_vectors[i][j] /= length;
        }
    }

    // Reduce to K by 2 using first two principal components
    for (int i = 0; i < K; i++) {
        result.push_back({ eigen_vectors[i][0], eigen_vectors[i][1] });
    }

    return result;
}