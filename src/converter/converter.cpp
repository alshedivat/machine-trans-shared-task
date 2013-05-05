#include "converter.h"
#include <stdexcept>
#include <iostream>

using std::cout;
using std::cerr;
using std::endl;
using std::exception;

Converter::Converter(const string& dict_file) {
  cout << "Initializing converter with " << dict_file << endl;
  std::ifstream input_vcb(dict_file.c_str());
  string input_line;
  while(std::getline(input_vcb, input_line)) {
    std::stringstream sstr(input_line);
    size_t index;
    string word;
    sstr >> index >> word;
    words_.push_back(word);
    dict_[word] = index;
  }
  cout << "Converter initialized" << endl;
}

Phrase Converter::ToIndex(const string& sentence) const {
  Phrase indices;
  std::stringstream sstr(sentence);
  string word;
  while(sstr >> word)
    try {
      indices.push_back(dict_.at(word));
    } catch (const exception& error) {
      cerr << "Fatal error occurred: " << error.what() << " with word: " << word << endl;
    }
  return indices;
}

string Converter::ToSentence(const Phrase& indices) const {
  string sentence;
  std::stringstream sstr(sentence);
  for (size_t index : indices)
    sstr << words_[index - 1] << " ";
  return sstr.str();
}

size_t Converter::DictSize() const {
  return words_.size();
}

