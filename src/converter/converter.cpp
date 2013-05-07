#include "converter.h"
#include <sstream>
#include <stdexcept>
#include <iostream>

using std::cout;
using std::cerr;
using std::endl;
using std::exception;
using std::getline;
using std::ifstream;
using std::stringstream;

Converter::Converter(const string& dict_file, PhraseTable* phrase_table)
  : phrase_table_(phrase_table) {
  cout << "Initializing converter with " << dict_file << endl;
  dict_path_ = dict_file;
  ifstream input_vcb(dict_file.c_str());
  string input_line;
  while(getline(input_vcb, input_line)) {
    stringstream sstr(input_line);
    size_t index;
    string word;
    sstr >> index >> word;
    words_.push_back(word);
    dict_[word] = index;
  }
  cout << "Converter initialized" << endl;
}

Phrase Converter::ToIndex(const string& sentence) {
  Phrase indices;
  stringstream sstr(sentence);
  string word;
  while(sstr >> word) {
    if (dict_.count(word) == 0) {
      cerr << "WARNING: There is no word " << word << " in the dictionary!"
           << " The word was appended to the dictionary." << endl;
      try {
        dict_[word] = words_.size() + 1;
        words_.push_back(word);
        if (phrase_table_ != NULL) {
          Phrase phrase;
          phrase.push_back(dict_.at(word));
          (*phrase_table_)[phrase] = vector<Translation>(1, Translation(phrase, 1.0));
        }
      } catch (const exception& error) {
        cerr << "Fatal error occured with adding new word: " << error.what() << " with word: " << word << endl;
      }
    }
    try {
      indices.push_back(dict_.at(word));
    } catch (const exception& error) {
      cerr << "Fatal error occurred: " << error.what() << " with word: " << word << endl;
    }
  }
  return indices;
}

string Converter::ToSentence(const Phrase& indices) const {
  string sentence;
  stringstream sstr(sentence);
  for (size_t index : indices)
    sstr << words_[index - 1] << " ";
  return sstr.str();
}

size_t Converter::DictSize() const {
  return words_.size();
}

