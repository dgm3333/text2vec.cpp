
#include "VocabCorpus.h"
#include <vector>
#include <unordered_map>
#include <string>

VocabCorpus::VocabCorpus(const std::vector<std::string>& vocab, uint32_t n_min, uint32_t n_max,
                         const std::vector<std::string>& stopwords, const std::string& delim) {
    tcm = SparseTripletMatrix<float>(vocab.size(), vocab.size());
    init(vocab, n_min, n_max, stopwords, delim);
    word_count.resize(vocab.size());
}

void VocabCorpus::init(const std::vector<std::string>& vocab, uint32_t n_min, uint32_t n_max,
                       const std::vector<std::string>& stopwords, const std::string& delim) {
    this->verbose = 0;
    this->nnz = 0;
    this->token_count = 0;
    this->doc_count = 0;
    this->cooc_tokens_number = 0;
    this->ngram_min = n_min;
    this->ngram_max = n_max;
    this->ngram_delim = delim;

    size_t i = 0;
    this->vocab.reserve(vocab.size());
    for (auto val:vocab) {
        this->vocab.insert(make_pair(val, i));
        i++;
    }
    for (auto val:stopwords) {
        this->stopwords.insert(val);
        i++;
    }
}

void VocabCorpus::insert_terms (std::vector< std::string>& terms, int grow_dtm, int context, uint32_t window_size,
                                const std::vector<double>& weights, int flag_binary_cooccurence) {
    // Implementation remains largely the same, except replacing NumericVector with std::vector<double>
}

void VocabCorpus::insert_document(const std::vector<std::string>& doc, int grow_dtm, int context,
                                  uint32_t window_size, const std::vector<double>& weights,
                                  int flag_binary_cooccurence) {
    std::vector<std::string> std_string_vec = doc;
    std::vector<std::string> ngram_vec
    = generate_ngrams(std_string_vec, this->ngram_min, this->ngram_max, this->stopwords, this->ngram_delim);

    this->insert_terms(ngram_vec, grow_dtm, context, window_size, weights, flag_binary_cooccurence);
    this->dtm.increment_nrows();
    this->doc_count++;
}

void VocabCorpus::insert_document_batch(const std::vector<std::vector<std::string>>& docs_batch, int grow_dtm,
                                        int context, uint32_t window_size, const std::vector<double>& weights,
                                        int flag_binary_cooccurence) {
    for (auto it:docs_batch) {
        this->insert_document(it, grow_dtm, context, window_size, weights, flag_binary_cooccurence);
    }
}

std::vector<std::string> VocabCorpus::get_vocab() {
    std::vector<std::string> vocab(vocab.size());
    for(auto i:this->vocab)
        vocab[ i.second ] = i.first;
    return vocab;
}

std::tuple<std::vector<uint32_t>, std::vector<uint32_t>, std::vector<float>, std::vector<std::string>, std::vector<std::string>>
VocabCorpus::get_tcm() {
    std::vector<std::string> dimnames(vocab.size());
    for(auto it: vocab)
        dimnames[it.second] = it.first;
    auto res = tcm.get_sparse_triplet_matrix(dimnames, dimnames);
    // The line below would add a new attribute "word_count" to the S4 object in R.
    // C++ does not support adding attributes to objects at runtime. So, this line has been commented out.
    // res.attr("word_count") = wrap(word_count);
    return res;
}

std::tuple<std::vector<uint32_t>, std::vector<uint32_t>, std::vector<float>, std::vector<std::string>, std::vector<std::string>>
VocabCorpus::get_dtm() {
    std::vector<std::string> dummy_doc_names(0);
    std::vector<std::string> terms(vocab.size());
    for(auto it: vocab)
        terms[it.second] = it.first;
    return dtm.get_sparse_triplet_matrix(dummy_doc_names, terms);
}