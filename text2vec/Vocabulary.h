#pragma once

// Vocabulary.h
// Copyright (C) 2015 - 2016  Dmitriy Selivanov
// This file is part of text2vec
//
// text2vec is free software: you can redistribute it and/or modify it
// under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 2 of the License, or
// (at your option) any later version.
//
// text2vec is distributed in the hope that will be useful, but
// WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with text2vec.  If not, see <http://www.gnu.org/licenses/>.

#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <string>
#include "utils.h"

class TermStat {
public:
	TermStat() {}
	TermStat(uint32_t term_id) : term_id(term_id), term_global_count(1), document_term_count(0) {}

	uint32_t term_id;
	uint32_t term_global_count;
	uint32_t document_term_count;
};

class Vocabulary {
public:
	Vocabulary() {}
	Vocabulary(uint32_t ngram_min, uint32_t ngram_max, const std::vector<std::string>& stopwords, const std::string& delim, int window_size) : ngram_min(ngram_min), ngram_max(ngram_max), document_count(0), token_count(0), window_size(window_size) {
		ngram_delim = delim;
		for (const auto& it : stopwords)
			this->stopwords.insert(it);
	}

	std::vector<TermStat> get_vocab_statistics() {
		size_t N = vocab.size();
		std::vector<TermStat> vocab_stats(N);
		size_t i = 0;
		for (const auto& it : vocab) {
			vocab_stats[i] = vocab_statistics[it.second];
			i++;
		}
		return vocab_stats;
	}

	void insert_terms(const std::vector<std::string>& terms) {
		int term_id;
		int window_size_ = (int)this->window_size;
		int terms_size = static_cast<int>(terms.size());
		if (window_size_ <= 0 || window_size_ > terms_size)
			window_size_ = terms_size;

		int index_window_start = 0;
		while (index_window_start + window_size_ <= terms_size) {
			this->temp_document_word_set.clear();
			for (int i = index_window_start; i < index_window_start + window_size_; i++) {
				this->temp_document_word_set.insert(terms[i]);
				auto term_iterator = this->vocab.find(terms[i]);
				if (term_iterator == this->vocab.end()) {
					term_id = static_cast<int>(this->vocab.size());
					this->vocab.insert(std::make_pair(terms[i], term_id));
					vocab_statistics.push_back(TermStat(term_id));
				}
				else {
					vocab_statistics[term_iterator->second].term_global_count++;
				}
				this->token_count++;
			}
			for (const auto& it : this->temp_document_word_set) {
				auto term_iterator = vocab.find(it);
				if (term_iterator != vocab.end())
					this->vocab_statistics[term_iterator->second].document_term_count++;
			}
			this->document_count++;
			index_window_start++;
		}
	}

	// Function to insert a batch of documents into the Vocabulary object
	void insert_document_batch(const std::vector<std::vector<std::string>>& document_batch) {
		std::vector<std::string> ngram_vec;
		for (const auto& doc : document_batch) {
			ngram_vec = generate_ngrams(doc, this->ngram_min, this->ngram_max, this->stopwords, this->ngram_delim);
			insert_terms(ngram_vec);
		}
	}

	// Function to insert a batch of documents into the Vocabulary object using a pointer to document tokens
	void insert_document_batch_ptr(std::vector<std::vector<std::string>>* doc_tokens) {
		std::vector<std::string> ngram_vec;
		for (const auto& doc : *doc_tokens) {
			ngram_vec = generate_ngrams(doc, this->ngram_min, this->ngram_max, this->stopwords, this->ngram_delim);
			insert_terms(ngram_vec);
		}
	}

	int get_document_count() { return this->document_count; }
	void increase_token_count() { token_count++; }

private:
	std::vector<TermStat> vocab_statistics;
	std::unordered_map<std::string, uint32_t> vocab;
	uint32_t ngram_min;
	uint32_t ngram_max;
	std::string ngram_delim;
	int document_count;
	uint32_t token_count;
	size_t window_size = 0;
	std::unordered_set<std::string> temp_document_word_set;
	std::unordered_set<std::string> stopwords;
};