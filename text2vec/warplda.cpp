#include "mcemlda/LDA.hpp"
#include <vector>
#include <random>
#include <unordered_map>

//extend base LDA class with R related methods
class R_LDA:public LDA {
public:

  void init(const std::vector<int> &z_old, const std::vector<int> &z_new, const std::vector<int> &seeds) {

    rng.seed((uint64_t)seeds[0], (uint64_t)seeds[1]);
    C_doc.resize(corpus.n_row_expected, n_topic);
    C_word.resize(corpus.n_col_expected, n_topic);
    C_all.resize(n_topic);
    C_local.resize(n_topic, 0);
    C_local_diff.resize(n_topic, 0);
    size_t j = 0;
    corpus.apply([&](Z& z, doc_index_t d, word_index_t w) {
      z.old_z = z_old[j];
      z.new_z = z_new[j];
      j++;
    });
    // Update C_doc & C_all
    C_doc.clear();
    corpus.apply([&](Z& z, doc_index_t d, word_index_t w) {
      C_doc.at(d, z.old_z) ++;
      C_all[z.old_z] ++;
      C_local[z.old_z] ++;
    });
    // Update C_word
    C_word.clear();
    corpus.apply<true>([&](Z& z, doc_index_t d, word_index_t w) {
      C_word.at(w, z.old_z) ++;
    });
  }

  void set_topic_word_count(const std::vector<std::vector<int>> &topic_word_count) {
    C_word.resize(topic_word_count[0].size(), topic_word_count.size());
    // Update C_word
    C_word.clear();
    for(word_index_t w = 0; w < C_word.nrow(); w++)
      for(topic_index_t t = 0; t < C_word.ncol(); t++)
        //topic_word_count matrix is n_topics * n_words, so indices "t", "w" swapped
        C_word.at(w, t) = topic_word_count[t][w];

  }

  // read in document-term matrix CSR format
  void r_read_corpus(const std::vector<int> &dims, const std::vector<int> &PP, const std::vector<int> &JJ, const std::vector<double> &XX) {
    int nr = dims[0];
    int nc = dims[1];

    int *P = PP.data();
    int *J = JJ.data();
    double *X = XX.data();

    // append csr
    for(auto r = 0; r < nr; r++) {
      size_t p1 = P[r];
      size_t p2 = P[r + 1];
      for(auto j = p1; j < p2; j++) {
        int idx = J[j];
        int cnt = X[j];
        for(auto k = 0; k < cnt; k++)
          corpus.append(r, idx, 0, 0);
      }
    }
    corpus.build_CSC_from_CSR(nr, nc);
  }

  std::vector<std::vector<int>> get_topic_word_count() {
    // size(C_word)  =  n_words * n_topics
    // we need n_topics * n_words
    size_t n_words = C_word.nrow();
    size_t n_topics = C_word.ncol();
    std::vector<std::vector<int>> topic_word_count(n_topics, std::vector<int>(n_words));

    for(word_index_t w = 0; w < n_words; w++)
      for(topic_index_t t = 0; t < n_topics; t++)
        // topics and words swapped - we return matrix of size n_topics * n_words
        topic_word_count[t][w] = C_word.at(w, t);

    return topic_word_count;
  }

  std::vector<std::vector<int>> get_doc_topic_count() {

    std::vector<std::vector<int>> doc_topic_count(C_doc.nrow(), std::vector<int>(C_doc.ncol()));

    for(doc_index_t d = 0; d < C_doc.nrow(); d++)
      for(topic_index_t t = 0; t < C_doc.ncol(); t++)
        doc_topic_count[d][t] = C_doc.at(d, t);

    return doc_topic_count;
  }

  std::vector<int> get_local_diff() {
    return C_local_diff;
  }

  std::vector<int> get_c_global() {
    return C_all;
  }

  void set_c_global(const std::vector<int> &r_c_all) {
    for(int i = 0; i < r_c_all.size(); i++)
      C_all[i] = r_c_all[i];
  }

  void reset_local_diff() {
    for(size_t i = 0; i < C_local_diff.size(); i++)
      C_local_diff[i] = 0;
  }
};
