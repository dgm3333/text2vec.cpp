// Vocabulary.cpp
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

#include "Vocabulary.h"

// Function to create the Vocabulary object and return a pointer to it
Vocabulary* cpp_vocab_create(uint32_t ngram_min, uint32_t ngram_max,
	const std::vector<std::string>& stopwords,
	const std::string& delim, int window_size) {
	Vocabulary* v = new Vocabulary(ngram_min, ngram_max, stopwords, delim, window_size);
	return v;
}

/*
// These aren't used in text2vec, but they were defined so code here for reference

// Function to insert a batch of documents into the Vocabulary object using a pointer
void cpp_vocabulary_insert_document_batch(Vocabulary* v, const std::vector<std::vector<std::string>>& document_batch) {
	v->insert_document_batch(document_batch);
}

// Function to insert a batch of documents into the Vocabulary object using a pointer to document tokens
void cpp_vocabulary_insert_document_batch_ptr(Vocabulary* v, const std::vector<std::vector<std::string>>& doc_tokens) {
	v->insert_document_batch_ptr(doc_tokens);
}


// Function to get the vocabulary statistics as a DataFrame using a pointer
DataFrame cpp_get_vocab_statistics(Vocabulary* v) {
	std::vector<TermStat> vocab_stats = v->get_vocab_statistics();
	size_t N = vocab_stats.size();
	std::vector<std::string> terms(N);
	std::vector<int> term_counts(N);
	std::vector<int> doc_counts(N);

	for (size_t i = 0; i < N; ++i) {
		terms[i] = vocab_stats[i].term;
		term_counts[i] = vocab_stats[i].term_global_count;
		doc_counts[i] = vocab_stats[i].document_term_count;
	}

	return DataFrame::create(_["term"] = terms, _["term_count"] = term_counts, _["doc_count"] = doc_counts);
}


// Function to get the total number of documents in the Vocabulary object using a pointer
int cpp_get_document_count(Vocabulary* v) {
	return v->get_document_count();
}

*/
