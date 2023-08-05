#pragma once

// SparseTripletMatrix.h
// Copyright (C) 2015 - 2016  Dmitriy Selivanov
// This file is part of text2vec
//
// text2vec is free software: you can redistribute it and/or modify it
// under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 2 of the License, or
// (at your option) any later version.
//
// text2vec is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with text2vec.  If not, see <http://www.gnu.org/licenses/>.

#include <string>
#include <vector>
#include <memory>
#include <unordered_map>
#include <tuple>

#define SPP_MIX_HASH

#include "text2vec.h"

using namespace std;

using S4 = std::tuple<std::vector<int32_t>, std::vector<int32_t>, std::vector<double>, std::vector<std::string>, std::vector<std::string>>;


template <>
struct hash<std::pair<uint32_t, uint32_t>>
{
	inline uint64_t operator()(const std::pair<uint32_t, uint32_t>& k) const
	{
		return (uint64_t) k.first << 32 | k.second;
	}
};




//get_sparse_triplet_matrix function includes two arguments for row and column names. This function returns a tuple that consists of:
//std::vector<uint32_t> for row indices (I)
//std::vector<uint32_t> for column indices (J)
//std::vector<T> for the values (X)
//std::vector<std::string> for row names
//std::vector<std::string> for column names
//The row and column names will need to be provided when calling get_sparse_triplet_matrix(), and the calling code will need to handle these return values appropriately.

template<typename T>
class SparseTripletMatrix {
public:
	SparseTripletMatrix():
		nrow(0), ncol(0), nnz(0) {};

	SparseTripletMatrix(size_t nrow, size_t ncol):
		nrow(nrow), ncol(ncol), nnz(0) {};

	inline void increment_nrows() {this->nrow++;};
	inline void increment_ncols() {this->ncol++;};

	inline uint32_t nrows() { return this->nrow;  };
	inline uint32_t ncols() { return this->ncol;  };
	inline size_t size()	{ return(this->sparse_container.size());   }
	void clear() { this->sparse_container.clear(); };
	void add(uint32_t i, uint32_t j, T increment) {
		this->sparse_container[make_pair(i, j)] += increment;	   // add / increment elements
	};

	S4 get_sparse_triplet_matrix(std::vector<std::string>& rownames, std::vector<std::string>& colnames);

private:
	size_t nrow;
	size_t ncol;
	size_t nnz;
	std::unordered_map< pair<uint32_t, uint32_t>, T> sparse_container;
};


template<typename T> S4 SparseTripletMatrix<T>::get_sparse_triplet_matrix(std::vector<std::string> &rownames, std::vector<std::string> &colnames) {

	size_t NNZ = this->size();

	// index vectors
	std::vector<int32_t> I(NNZ), J(NNZ);	  //IntegerVector
	// value vector
	std::vector<double> X(NNZ);			   //NumericVector

	size_t n = 0;
	for(auto it : sparse_container) {
		I[n] = it.first.first;
		J[n] = it.first.second;
		X[n] = it.second;
		n++;
	}

	return std::make_tuple(I, J, X, rownames, colnames);
}
