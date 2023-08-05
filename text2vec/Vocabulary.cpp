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

Vocabulary* vocab_create(uint32_t ngram_min,
                         uint32_t ngram_max,
                         const vector<string> stopwords_R,
                         const string delim,
                         int window_size) {
  Vocabulary *v = new Vocabulary(ngram_min, ngram_max, stopwords_R, delim, window_size);
  return v;
}

void vocabulary_insert_document_batch(Vocabulary* ptr, const vector<vector<string>> document_batch) {
  ptr->insert_document_batch(document_batch);
}

void vocabulary_insert_document_batch_xptr(Vocabulary* ptr, vector<vector<string>>* document_batch_ptr) {
  ptr->insert_document_batch_ptr(document_batch_ptr);
}

DataFrame get_vocab_statistics(Vocabulary* ptr) {
  return ptr->get_vocab_statistics();
}

int get_document_count(Vocabulary* ptr) {
  return ptr->get_document_count();
}
