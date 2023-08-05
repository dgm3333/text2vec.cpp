// text2vec.h

#pragma once
#ifndef TEXT2VEC_H
#define TEXT2VEC_H

#include <string>
#include <vector>
#include <unordered_map>

std::string tolower(const std::string& s);
std::vector<std::string> word_tokenizer(const std::string& s, char delim = ' ');
std::vector<std::string> itoken(const std::vector<std::string>& txt);
std::vector<std::vector<std::string>> split_into(const std::vector<std::string>& vec, int N);

std::unordered_map<std::string, int> create_vocabulary(const std::vector<std::string>& tokens);
std::unordered_map<std::string, int> create_vocabulary(const std::vector<std::string>& txt, uint32_t ngram_min, uint32_t ngram_max, std::unordered_set<std::string>& stopwords, const std::string& ngram_delim);
std::unordered_map<std::string, int> create_vocabulary(const std::vector<std::pair<std::string, std::string>>& txt_ids, uint32_t ngram_min, uint32_t ngram_max, std::unordered_set<std::string>& stopwords, const std::string& ngram_delim);

//std::unordered_map<std::string, int> create_vocabulary(const std::vector<std::pair<std::string, std::string>>& txt_ids, uint32_t ngram_min = 1, uint32_t ngram_max = 1, std::unordered_set<std::string>& stopwords = {}, const std::string& ngram_delim = " ");

std::unordered_map<std::string, int> combine_vocabularies(const std::unordered_map<std::string, int>& vocab1, const std::unordered_map<std::string, int>& vocab2);

size_t get_document_count(const std::unordered_map<std::string, int>& vocab);
int get_document_countv2(const std::unordered_map<std::string, int>& vocab);

std::vector<std::pair<std::string, std::string>> get_test_iterator(const std::vector<std::string>& txt, const std::vector<std::string>& ids);

typedef std::vector<std::string>(*TokenizerFunc)(const std::string&, char);
typedef std::string(*PreprocessFunc)(const std::string&);
std::vector<std::pair<std::string, std::string>> get_test_iterator(const std::vector<std::string>& txt, const std::vector<std::string>& ids, PreprocessFunc preprocess_function, TokenizerFunc tokenizer, float& progress);

#endif // TEXT2VEC_H