#pragma once

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

#include "SparseTripletMatrix.h"
#include <unordered_set>
#include <vector>
#include <string>

class Corpus {
public:

	/*
	//void insert_terms(std::vector<std::string>& terms);
	void insert_terms(std::vector<std::string>& terms, int grow_dtm, int context, uint32_t window_size, const std::vector<double>& weights);

	//	void insert_document(const std::vector<std::string>& terms);
	void insert_document(const std::vector<std::string>& doc, int grow_dtm, int context, uint32_t window_size, const std::vector<double>& weights, int flag_binary_cooccurrence = 0);

	//  void insert_document_batch(const std::vector<std::vector<std::string>>& docs_batch);
	void insert_document_batch(const std::vector<std::vector<std::string>>& docs_batch, int grow_dtm, int context, uint32_t window_size, const std::vector<double>& weights, int flag_binary_cooccurrence = 0);
	*/

	SparseTripletMatrix<int> get_dtm_triplet() { return dtm; }

	void clear_tcm() { tcm.clear(); }
	size_t get_tcm_size() { return tcm.size(); }
	SparseTripletMatrix<float> get_tcm_triplet() { return tcm; }

	uint32_t get_doc_count() { return doc_count; };				// total number of documents in corpus
	uint32_t get_token_count() { return token_count; };			// total number of tokens in corpus



protected:
	// token counter
	uint32_t token_count;
	size_t nnz;
	//document counter
	uint32_t doc_count;

	// ngram bounds
	uint32_t ngram_min;
	uint32_t ngram_max;
	// ngram concatenation delimiter
	std::string ngram_delim;

	// uint32_t window_size;
	// stopwords
	std::unordered_set<std::string> stopwords;
	// documents
	SparseTripletMatrix<int> dtm;
	std::vector<int> word_count;
	std::unordered_map<std::string, size_t> word_index;

	//#####Glove related
	uint64_t cooc_tokens_number;

	// term cooccurence matrix
	SparseTripletMatrix<float> tcm;

	// inline float weighting_fun(uint32_t offset) {
	//   return 1.0 / (float)offset;
	// }

};

/*
void Corpus::insert_terms(std::vector<std::string>& terms, int grow_dtm, int context, uint32_t window_size, const std::vector<double>& weights) {
	for (const auto& term : terms) {
		if (stopwords.find(term) == stopwords.end()) {
			if (word_index.find(term) == word_index.end()) {
				word_index[term] = word_count.size();
				word_count.push_back(1);
			}
			else {
				word_count[word_index[term]]++;
			}
			token_count++;
		}
	}
}

void Corpus::insert_document(const std::vector<std::string>& doc, int grow_dtm, int context, uint32_t window_size, const std::vector<double>& weights, int flag_binary_cooccurrence) {
	std::unordered_set<size_t> doc_word_indices;
	for (const auto& term : doc) {
		if (stopwords.find(term) == stopwords.end()) {
			auto it = word_index.find(term);
			if (it != word_index.end()) {
				uint32_t word_idx = (uint32_t)it->second;
				dtm.add(doc_count, word_idx, 1);
				doc_word_indices.insert(word_idx);
			}
		}
	}
	// Update word_count based on unique word indices in this document
	for (auto word_idx : doc_word_indices) {
		word_count[word_idx]++;
	}
	doc_count++;
}


void Corpus::insert_document_batch(const std::vector<std::vector<std::string>>& docs_batch, int grow_dtm, int context, uint32_t window_size, const std::vector<double>& weights, int flag_binary_cooccurrence) {
	for (const auto& doc : docs_batch) {
		insert_document(doc, grow_dtm, context, window_size, weights, flag_binary_cooccurrence);
	}
}

*/

