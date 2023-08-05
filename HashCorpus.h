#include "Corpus.h"
#include "SparseTripletMatrix.h"
#include <pmurhashAPI.h>
#include <vector>
#include <string>
#include <unordered_map>
#include <memory>

class HashCorpus: public Corpus {
public:
  // constructor
  HashCorpus(uint32_t size,
             uint32_t ngram_min, uint32_t ngram_max,
             //uint32_t win_size,
             int use_signed_hash);
  // total number of tokens in corpus
  int get_token_count() {return this -> token_count;};
  int get_doc_count() { return this -> doc_count; };

  void clear_tcm() {this->tcm.clear();};
  size_t get_tcm_size() {return this->tcm.size();};

  // implements hashing trick
  void insert_terms (std::vector<std::string> &terms, int grow_dtm, int context,
                     uint32_t window_size, const std::vector<double> &weights);

  void insert_document(const std::vector<std::string>& doc, int grow_dtm, int context,
                       uint32_t window_size, const std::vector<double> &weights);
  void insert_document_batch(const std::vector<std::vector<std::string>>& docs_batch, int grow_dtm,
                             int context, uint32_t window_size, const std::vector<double> &weights);

  // get term cooccurrence matrix
  std::tuple<std::vector<uint32_t>, std::vector<uint32_t>, std::vector<double>, std::vector<std::string>, std::vector<std::string>> get_tcm() {
    std::vector<std::string> dummy_names;
    return tcm.get_sparse_triplet_matrix(dummy_names, dummy_names);
  }

  std::tuple<std::vector<uint32_t>, std::vector<uint32_t>, std::vector<double>, std::vector<std::string>, std::vector<std::string>> get_dtm_triplet() {
    std::vector<std::string> dummy_names;
    return dtm.get_sparse_triplet_matrix(dummy_names, dummy_names);
  };

  // interface to document-term matrix construction
  std::tuple<std::vector<uint32_t>, std::vector<uint32_t>, std::vector<double>, std::vector<std::string>, std::vector<std::string>> get_dtm() { return get_dtm_triplet();};

private:
  uint32_t buckets_size;
  int signed_hash;
};
