#pragma once

// VocabCorpus.h

#include <string>
#include <vector>
#include <unordered_map>
#include "Corpus.h"
#include "Vocabulary.h"

//-----------------------------------------------------------------
// VocabCorpus class definitions
//-----------------------------------------------------------------
class VocabCorpus : public Corpus {
public:
	// constructor with window_size for term cooccurrence matrix
	VocabCorpus(const std::vector<std::string>& vocab, uint32_t n_min, uint32_t n_max,
				const std::vector<std::string>& stopwords, const std::string& delim);

	void insert_terms(std::vector<std::string>& terms, int grow_dtm, int context, uint32_t window_size, const std::vector<double>& weights, int flag_binary_cooccurrence = 0);

	void insert_document(const std::vector<std::string>& doc, int grow_dtm, int context, uint32_t window_size, const std::vector<double>& weights, int flag_binary_cooccurrence = 0);

	void insert_document_batch(const std::vector<std::vector<std::string>>& docs_batch, int grow_dtm, int context, uint32_t window_size, const std::vector<double>& weights, int flag_binary_cooccurrence = 0);

	// total number of tokens in corpus
	int get_token_count() { return token_count; };
	int get_doc_count() { return doc_count; };

	void clear_tcm() { tcm.clear(); };
	size_t get_tcm_size() { return tcm.size(); };

	std::vector<std::string> get_vocab();

	// get term cooccurrence matrix
	S4 get_tcm();

	S4 get_dtm();


private:
	int verbose;
	// vocabulary
	std::unordered_map<std::string, uint32_t> vocab;
	std::unordered_set<std::string> stopwords;
	uint32_t ngram_min;
	uint32_t ngram_max;
	std::string ngram_delim;
	int token_count;
	int doc_count;
	// Other data members as needed

	void init(const std::vector<std::string>& vocab, uint32_t n_min, uint32_t n_max, const std::vector<std::string>& stopwords, const std::string& delim);
	// Other helper functions as needed
};
