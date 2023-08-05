#pragma once

#include <string>
#include <vector>
#include <unordered_set>


uint32_t fast_int_hash(uint32_t a);
const std::string currentDateTime();
std::vector<std::string> stringVectorToStdVector(std::vector<std::string> terms_raw);
std::vector<std::string> generate_ngrams(const std::vector< std::string>& terms, const uint32_t ngram_min, const uint32_t ngram_max, std::unordered_set<std::string>& stopwords, const std::string ngram_delim);
std::vector<std::string> fixed_char_tokenizer(const std::string& s, char delim);
std::vector<std::vector < std::string> > cpp_fixed_char_tokenizer(std::vector<std::string> x, char delim);