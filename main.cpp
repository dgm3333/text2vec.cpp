// text2vec.cpp.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <iostream>
#include <vector>
#include <string>
#include <unordered_set>
#include "helper_functions.h"
#include "text2vec.h" // text2vec C++ library


using namespace std;

unordered_set<string> stopwords = { "a", "the" };

bool testVocabulary() {
    const int N_WORKER = 4;

    // Replace with actual data
    vector<string> txt = {/*movie_review reviews for train_ind*/ };
    vector<string> ids = {/*movie_review ids for train_ind*/ };

    auto txt_splits = split_into(txt, N_WORKER);
    auto ids_splits = split_into(ids, N_WORKER);

    auto iterator = get_test_iterator(txt, ids, tolower, word_tokenizer, false);
    auto vocab_1 = create_vocabulary(iterator);

    bool (get_document_count(vocab_1) == txt.size());
}

bool testCombineVocabularies() {
    // Replace with actual data
    vector<string> txt = {/*movie_review reviews*/ };
    vector<string> ids = {/*movie_review ids*/ };

    auto iterator = get_test_iterator(txt, ids, tolower, word_tokenizer, false);
    auto vocab = create_vocabulary(iterator, 1, 2, stopwords, "_");

    // Split data
    vector<string> txt_1 = {/*movie_review reviews for train_ind_1*/ };
    vector<string> ids_1 = {/*movie_review ids for train_ind_1*/ };

    vector<string> txt_2 = {/*movie_review reviews for train_ind_2*/ };
    vector<string> ids_2 = {/*movie_review ids for train_ind_2*/ };

    auto iterator_1 = get_test_iterator(txt_1, ids_1, tolower, word_tokenizer, false);
    auto vocab_1 = create_vocabulary(iterator_1, 1, 2, stopwords, "_");

    auto iterator_2 = get_test_iterator(txt_2, ids_2, tolower, word_tokenizer, false);
    auto vocab_2 = create_vocabulary(iterator_2, 1, 2, stopwords, "_");

    auto vocab_combined = combine_vocabularies(vocab_1, vocab_2);

    return (vocab_combined == vocab);
}

int main() {

    std::cout << testVocabulary() << std::endl;
    std::cout << testCombineVocabularies() << std::endl;
    return 0;
}

