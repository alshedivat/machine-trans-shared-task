#ifndef CONVERTER_CONVERTER_H
#define CONVERTER_CONVERTER_H

#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <unordered_map>
#include "../phrasetable/phrasetable.h"

using std::string;
using std::vector;

class Converter {
 public:
  typedef std::unordered_map<string, size_t> Dict;
  typedef std::vector<string> WordsList;

 public:
  Converter(const string& dict_file, PhraseTable* phrase_table = NULL);

  Phrase ToIndex(const string& sentence);

  string ToSentence(const Phrase& indices) const;

  size_t DictSize() const;

 private:
  PhraseTable* phrase_table_;
  string dict_path_;
  Dict dict_;
  WordsList words_;

};

#endif // CONVERTER_CONVERTER_H

