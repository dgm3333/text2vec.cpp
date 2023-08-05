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

#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>

class TermStat {
public:
  TermStat() {};
  TermStat(uint32_t term_id):
  term_id(term_id), term_global_count(1),
  document_term_count(0) {};

  uint32_t term_id;
  // term count in all corpus
  uint32_t term_global_count;
  // number of documents, which contain this term
  uint32_t document_term_count;
};

struct DataFrame {
    std::vector<std::string> term;
    std::vector<uint32_t> term_count;
    std::vector<uint32_t> doc_count;
};

class Vocabulary {
public:
  Vocabulary();
  Vocabulary(uint32_t ngram_min,
                         uint32_t ngram_max,
                         const std::vector<std::string>& stopwords,
                         const std::string& delim,
                         int window_size_):
    ngram_min(ngram_min), ngram_max(ngram_max),
    document_count(0), token_count(0), window_size(window_size_) {
    ngram_delim = delim;
    for(auto it:stopwords)
      this->stopwords.insert(it);
  };
  
  DataFrame get_vocab_statistics();
  void insert_terms(const std::vector<std::string> &terms);
  void insert_document_batch(const std::vector<std::vector<std::string>>& document_batch);
  void insert_document_batch_ptr(std::vector<std::vector<std::string>> *doc_tokens);
  int get_document_count() {return(this->document_count);};
  void increase_token_count() {token_count++;};

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
