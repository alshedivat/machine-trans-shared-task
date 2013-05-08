#ifndef CONVERTER_CONVERTER_H
#define CONVERTER_CONVERTER_H

#include <boost/thread.hpp>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <unordered_map>
#include "../phrasetable/phrasetable.h"

class Converter {
 public:
  typedef std::unordered_map<string, size_t> Dict;
  typedef std::vector<string> WordsList;

 public:
  Converter(const std::string& dict_file);

  Phrase ToIndex(const std::string& sentence);

  string ToSentence(const Phrase& indices) const;

  // size_t DictSize() const;

 private:
  void AddNewWordToDict(const std::string& word);
  boost::mutex mutex_;
  std::string dict_path_;
  Dict dict_;
  WordsList words_;

};

#endif // CONVERTER_CONVERTER_H

