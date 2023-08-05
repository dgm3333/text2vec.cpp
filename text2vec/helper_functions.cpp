#include <string>
#include <vector>
#include <iostream>
#include <unordered_map>
#include <unordered_set>
#include <sstream>
#include <algorithm>
#include <locale>
#include <tuple>
#include "utils.h"
#include "helper_functions.h"

using namespace std;

// tolower
string tolower(const string& s) {
	string out = s;
	transform(out.begin(), out.end(), out.begin(), [](unsigned char c) { return std::tolower(c); });
	return out;
}

// word_tokenizer
vector<string> word_tokenizer(const string& s, char delim) {
	vector<string> tokens;
	stringstream ss(s);
	string item;
	while (getline(ss, item, delim)) {
		tokens.push_back(item);
	}
	return tokens;
}

// itoken
vector<string> itoken(const vector<string>& txt) {
	vector<string> tokens;
	for (const auto& s : txt) {
		auto words = word_tokenizer(tolower(s));
		tokens.insert(tokens.end(), words.begin(), words.end());
	}
	return tokens;
}


// create_vocabulary
unordered_map<string, int> create_vocabulary(const vector<string>& tokens) {
	unordered_map<string, int> vocab;
	for (const auto& token : tokens) {
		++vocab[token];
	}
	return vocab;
}
std::unordered_map<std::string, int> create_vocabulary(const std::vector<std::string>& txt, uint32_t ngram_min, uint32_t ngram_max, std::unordered_set<std::string>& stopwords, const std::string& ngram_delim) {
	unordered_map<string, int> vocab;
	for (const auto& s : txt) {
		auto tokens = word_tokenizer(tolower(s));
		auto ngrams = generate_ngrams(tokens, ngram_min, ngram_max, stopwords, ngram_delim);
		for (const auto& ngram : ngrams) {
			++vocab[ngram];
		}
	}
	return vocab;
}
std::unordered_map<std::string, int> create_vocabulary(const std::vector<std::pair<std::string, std::string>>& txt_ids, uint32_t ngram_min, uint32_t ngram_max, std::unordered_set<std::string>& stopwords, const std::string& ngram_delim) {

	std::unordered_map<std::string, int> vocab;

	for (const auto& [txt, id] : txt_ids) {
		auto tokens = word_tokenizer(tolower(txt));
		auto ngrams = generate_ngrams(tokens, ngram_min, ngram_max, stopwords, ngram_delim);

		for (const auto& ngram : ngrams) {
			++vocab[ngram];
		}
	}

	return vocab;
}

// combine_vocabularies
// This version assumes that vocab1 contains all the words in vocab2...
std::unordered_map<std::string, int> combine_vocabulariesv0(const std::unordered_map<std::string, int>& vocab1, const std::unordered_map<std::string, int>& vocab2) {
	unordered_map<string, int> vocab_combined = vocab1;
	for (const auto& [word, count] : vocab2) {
		vocab_combined[word] += count;
	}
	return vocab_combined;
}
// combine_vocabularies
std::unordered_map<std::string, int> combine_vocabularies(const std::unordered_map<std::string, int>& vocab1, const std::unordered_map<std::string, int>& vocab2) {
	std::unordered_map<std::string, int> vocab_combined = vocab1;
	for (const auto& [word, count] : vocab2) {
		if (vocab_combined.find(word) == vocab_combined.end()) {
			vocab_combined[word] = count;
		}
		else {
			vocab_combined[word] += count;
		}
	}
	return vocab_combined;
}



size_t get_document_count(const std::unordered_map<std::string, int>& vocab) {
	return vocab.size();
}
int get_document_countv2(const std::unordered_map<std::string, int>& vocab) {
	int count = 0;
	for (const auto& [word, word_count] : vocab) {
		count += word_count;
	}
	return count;
}

std::vector<std::pair<std::string, std::string>> get_test_iterator(const std::vector<std::string>& txt, const std::vector<std::string>& ids) {
	std::vector<std::pair<std::string, std::string>> result;
	for (size_t i = 0; i < txt.size() && i < ids.size(); ++i) {
		auto tokens = word_tokenizer(tolower(txt[i]));
		for (const auto& token : tokens) {
			result.push_back(std::make_pair(token, ids[i]));
		}
	}
	return result;
}



std::vector<std::pair<std::string, std::string>> get_test_iterator(
	const std::vector<std::string>& txt,
	const std::vector<std::string>& ids,
	PreprocessFunc preprocess_function,
	TokenizerFunc tokenizer,
	float& progress) {

	std::vector<std::pair<std::string, std::string>> result;
	for (size_t i = 0; i < txt.size() && i < ids.size(); ++i) {
		auto preprocessed_txt = preprocess_function(txt[i]);
		auto tokens = tokenizer(preprocessed_txt, ' ');

		for (const auto& token : tokens) {
			result.push_back(std::make_pair(token, ids[i]));
		}

		if (i % 1000 == 0) {
			if (progress == -1) {
				std::cout << "Progress: " << (float)i / (float)txt.size() << "\n";
			}
			else {
				progress = (float)i / (float)txt.size();
			}
		}
	}

	return result;
}