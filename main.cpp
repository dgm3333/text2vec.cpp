// text2vec.cpp.cpp : This file contains the 'main' function. Program execution begins and ends there.
//


#include <filesystem>
#include <fstream>
#include <iostream>
#include <iostream>
#include <vector>
#include <string>
#include <unordered_set>
#include "helper_functions.h"
#include "text2vec.h" // text2vec C++ library
#include "utils.h"


using namespace std;

// Potential lists of stopwords
// https://github.com/stopwords-iso/stopwords-en/blob/master/stopwords-en.json					// words in alphabetical order
// https://gist.github.com/sebleier/554280
// https://github.com/quanteda/stopwords/blob/master/data-raw/marimo/stopwords_en.yml			// grouped by type

unordered_set<string> stopwords = { "a", "the" };


// This will load data
// D:\Large Language Models\Large Movie Review Dataset (aclImdb_v1
void loadFilesFromDirectory(const std::string& directoryPath, std::vector<std::string>& fileContents, std::vector<std::string>& fileNames, int startFileIndex = 0, int numFilesToImport = INT_MAX) {
	int fileCount = 0;
	for (const auto& entry : std::filesystem::directory_iterator(directoryPath)) {
		if (!entry.is_regular_file()) {
			continue; // Skip directories or special files
		}

		if (fileCount < startFileIndex) {
			++fileCount;
			continue; // Skip files before the start index
		}

		if (fileCount >= startFileIndex + numFilesToImport) {
			break; // Stop importing after reaching the desired number of files
		}

		std::string fileName = entry.path().filename().string();
		std::ifstream file(entry.path());

		if (file.is_open()) {
			std::string fileContent((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
			fileContents.push_back(fileContent);
			fileNames.push_back(fileName);
			file.close();
		}
		else {
			std::cerr << "Error opening file: " << fileName << std::endl;
		}

		++fileCount;
	}
}



bool testVocabulary(std::string directoryPath) {
	const int N_WORKER = 4;
	float progress = -1.0f;

	int startFileIndex = 0;
	int numFilesToImport = 100;

	// Load in data
	std::vector<std::string> txt = {/*movie_review reviews for train_ind*/ };
	std::vector<std::string> ids = {/*movie_review ids for train_ind*/ };
	loadFilesFromDirectory(directoryPath, txt, ids, startFileIndex, numFilesToImport);

	auto txt_splits = split_into(txt, N_WORKER);
	auto ids_splits = split_into(ids, N_WORKER);

	std::vector<std::pair<std::string, std::string>> iterator = get_test_iterator(txt, ids, tolower, word_tokenizer, progress);
	std::unordered_map<std::string, int> vocab_1 = create_vocabulary(iterator, 1, 1, stopwords, " ");

	// test output - list the words which have occurred at least min_count times
	int min_count = 3;
	for (auto& v : vocab_1) {
		if (v.second < min_count)
			continue;
		std::cout << v.first << " " << v.second << std::endl;
	}

	return (get_document_count(vocab_1) == txt.size());
}


// Testing if single load of Vocab == same as separately loading two halves then combining (Vocab1 + Vocab2).
bool testCombineVocabularies(std::string directoryPath) {
	const int N_WORKER = 4;
	float progress = -1.0f;


	// Load in data
	std::vector<std::string> txt = {/*movie_review reviews*/ };
	std::vector<std::string> ids = {/*movie_review ids*/ };
	loadFilesFromDirectory(directoryPath, txt, ids, 0, 100);
	std::vector<std::pair<std::string, std::string>> iterator = get_test_iterator(txt, ids, tolower, word_tokenizer, progress);
	std::unordered_map<std::string, int> vocab = create_vocabulary(iterator, 1, 2, stopwords, "_");

	// Split data
	vector<string> txt_1 = {/*movie_review reviews for train_ind_1*/ };
	vector<string> ids_1 = {/*movie_review ids for train_ind_1*/ };
	loadFilesFromDirectory(directoryPath, txt_1, ids_1, 0, 50);
	std::vector<std::pair<std::string, std::string>> iterator_1 = get_test_iterator(txt_1, ids_1, tolower, word_tokenizer, progress);
	std::unordered_map<std::string, int> vocab_1 = create_vocabulary(iterator_1, 1, 2, stopwords, "_");


	vector<string> txt_2 = {/*movie_review reviews for train_ind_2*/ };
	vector<string> ids_2 = {/*movie_review ids for train_ind_2*/ };
	loadFilesFromDirectory(directoryPath, txt_2, ids_2, 50, 50);
	std::vector<std::pair<std::string, std::string>> iterator_2 = get_test_iterator(txt_2, ids_2, tolower, word_tokenizer, progress);
	std::unordered_map<std::string, int> vocab_2 = create_vocabulary(iterator_2, 1, 2, stopwords, "_");

	std::unordered_map<std::string, int> vocab_combined = combine_vocabularies(vocab_1, vocab_2);

	// test output - list the words which have occurred at least min_count times
	int min_count = 3;
	for (auto& v : vocab_combined) {
		if (v.second < min_count)
			continue;
		std::cout << v.first << " " << v.second << std::endl;
	}

	std::cout << "single load (vocab0).size() == " << vocab.size() << "\nvocab1+vocab2 combined.size() == " << vocab_combined.size() << "\n";
	return (vocab_combined == vocab);
}

int main() {

	std::string directoryPath = R"(D:\Large Language Models\Large Movie Review Dataset (aclImdb_v1\aclImdb\test\neg)";


	std::cout << testVocabulary(directoryPath) << std::endl;
	std::cout << "***********************************************************************" << std::endl;
	std::cout << "Testing if single load of Vocab == same as separately loading then combining (Vocab1 + Vocab2). Result = " << testCombineVocabularies(directoryPath) << std::endl;

	return 0;
}

