#include "converter.h"

Converter::Converter(const string& dict_file) {
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
}

vector<size_t> Converter::ToIndex(const string& sentence) {
  vector<size_t> indices;
  std::stringstream sstr(sentence);
  string word;
  while(sstr >> word)
    indices.push_back(dict_[word]);
  return indices;
}

string Converter::ToSentence(const vector<size_t>& indices) {
  string sentence;
  std::stringstream sstr(sentence);
  for (size_t index : indices)
    sstr << words_[index - 1] << " ";
  return sstr.str();
}

