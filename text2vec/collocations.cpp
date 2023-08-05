#include <unordered_set>
#include <vector>
#include <string>
#include <memory>

std::shared_ptr<std::unordered_set<std::string>> create_xptr_unordered_set(const std::vector<std::string>& x) {
  auto res = std::make_shared<std::unordered_set<std::string>>();
  for (const auto& it : x) {
    res->insert(it);
  }
  return res;
}

std::vector<std::vector<std::string>> collapse_collocations_cpp(const std::vector<std::vector<std::string>>& docs,
                                                                std::shared_ptr<std::unordered_set<std::string>> collocations,
                                                                std::shared_ptr<std::unordered_set<std::string>> stopwords,
                                                                const std::string& sep) {
  size_t input_size = docs.size();
  std::vector<std::vector<std::string>> res(input_size);

  for (uint32_t i = 0; i < input_size; i++) {
    const auto& termsR = docs[i];
    std::vector<std::string> terms;
    for (const auto& it : termsR) {
      if(stopwords->find(it) == stopwords->end())
        terms.push_back(it);
    }
    size_t tsize = terms.size();
    std::vector<std::string> out_terms(tsize);
    std::string w0 = sep;
    size_t k = 0;

    if(tsize > 0) {
      uint32_t j = 1;
      std::string w1 = terms[j - 1];
      while(j < tsize) {
        const std::string& w2 = terms[j];
        std::string collocation_candidate = w1 + sep + w2;
        if(collocations->find(collocation_candidate) != collocations->end()) {
          w1 = collocation_candidate;
        } else {
          std::string collocation_candidate_2 = w0 + sep + w1;
          auto it2 = collocations->find(collocation_candidate_2);
          if(it2 != collocations->end() && k >= 1) {
            out_terms[k - 1] = collocation_candidate_2;
          }
          else {
            out_terms[k] = w1;
            w0 = w1;
            k++;
          }
          w1 = w2;
        }
        j++;
      }
      std::string collocation_candidate_2 = w0 + sep + w1;
      auto it2 = collocations->find(collocation_candidate_2);
      if(it2 != collocations->end()) {
        out_terms[k - 1] = collocation_candidate_2;
      }
      else {
        out_terms[k] = w1;
        k++;
      }
    }
    std::vector<std::string> r_out_terms(k);
    for(size_t j = 0; j < k; j++ )
      r_out_terms[j] = out_terms[j];
    res[i] = r_out_terms;
  }
  return res;
}
