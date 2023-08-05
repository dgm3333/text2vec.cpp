#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <sstream>
#include <algorithm>
#include <locale>
#include <tuple>
#include "utils.h"

using namespace std;

// tolower
string tolower(const string& s) {
    string out = s;
    transform(out.begin(), out.end(), out.begin(), [](unsigned char c) { return std::tolower(c); });
    return out;
}

// word_tokenizer
vector<string> word_tokenizer(const string& s, char delim = ' ') {
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

// split_into
vector<vector<string>> split_into(const vector<string>& vec, int N) {
    vector<vector<string>> splits(N);
    for (size_t i = 0; i < vec.size(); ++i) {
        splits[i % N].push_back(vec[i]);
    }
    return splits;
}

// create_vocabulary
unordered_map<string, int> create_vocabulary(const vector<string>& tokens) {
    unordered_map<string, int> vocab;
    for (const auto& token : tokens) {
        ++vocab[token];
    }
    return vocab;
}
unordered_map<string, int> create_vocabulary(const vector<string>& txt, uint32_t ngram_min, uint32_t ngram_max, std::unordered_set<std::string>& stopwords, const std::string& ngram_delim) {
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