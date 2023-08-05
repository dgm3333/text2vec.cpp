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
std::unordered_map<std::string, int> combine_vocabularies(const std::unordered_map<std::string, int>& vocab1, const std::unordered_map<std::string, int>& vocab2);

#endif // TEXT2VEC_H