#include "HashCorpus.h"
#include <iostream>
#include <cstring>

// seeds for hashing trick
const uint32_t MURMURHASH3_HASH_SEED = 3120602769LL;
const uint32_t MURMURHASH3_SIGN_SEED = 79193439LL;

// feature hash
uint32_t murmurhash3_hash(std::string &str) {
  return PMurHash32(MURMURHASH3_HASH_SEED, str.c_str(), str.size());
}

// feature sign hash
int murmurhash3_sign(const std::string &str) {
  return (int)PMurHash32(MURMURHASH3_SIGN_SEED, str.c_str(), str.size());
}

//-----------------------------------------------------------------
// implementation HashCorpus methods
//-----------------------------------------------------------------
HashCorpus::HashCorpus(uint32_t size,
                       uint32_t ngram_min, uint32_t ngram_max,
                       int use_signed_hash) {
  doc_count = 0;
  token_count = 0;
  buckets_size = size;
  signed_hash = use_signed_hash;
  this->ngram_min = ngram_min;
  this->ngram_max = ngram_max;
  this->ngram_delim = "_";
  dtm = SparseTripletMatrix<int>( 0, size );
  tcm = SparseTripletMatrix<float>( size, size );
  word_count.resize(size);
}

void HashCorpus::insert_terms(std::vector<std::string> &terms, int grow_dtm, int context,
                              uint32_t window_size, const std::vector<double> &weights) {
  uint32_t term_index, context_term_index;

  size_t K = terms.size();
  size_t i = 0;
  float increment = 0.0;

  for(auto term: terms) {
    this->token_count++;
    term_index = murmurhash3_hash(term) % buckets_size;
    word_count[term_index]++;
    if(grow_dtm) {
      int wcnt = 1;
      if(signed_hash && murmurhash3_sign(term) < 0)
        wcnt = -1;
      dtm.add(doc_count, term_index, wcnt);
    }
    // code for cooccurrence
    // ...
    // your existing code for cooccurrence
    // ...
    i++;
  }
}

void HashCorpus::insert_document(const std::vector<std::string>& doc, int grow_dtm, int context,
                                 uint32_t window_size, const std::vector<double> &weights) {
  std::vector<std::string> ngram_vec = generate_ngrams(doc, this->ngram_min, this->ngram_max, this->stopwords, this->ngram_delim);

  this->insert_terms(ngram_vec, grow_dtm, context, window_size, weights);
  this->dtm.increment_nrows();
  this->doc_count++;
}

void HashCorpus::insert_document_batch(const std::vector<std::vector<std::string>>& docs_batch,
                                       int grow_dtm, int context,
                                       uint32_t window_size, const std::vector<double> &weights) {
  for (auto it : docs_batch)
    insert_document(it, grow_dtm, context, window_size, weights);
}
