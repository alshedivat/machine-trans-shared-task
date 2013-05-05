#ifndef PHRASETABLE_PHRASETABLE_H
#define PHRASETABLE_PHRASETABLE_H

#include <unordered_map>
#include <string>
#include <vector>

using std::unordered_map;
using std::string;
using std::vector;

typedef size_t Word;
typedef vector< Word > Phrase;

struct Translation {
  Translation(Phrase phrase, double probability)
      : dest(phrase), prob(probability) {
  }

  Phrase dest;
  double prob;
};

struct CompareTranslations {
  bool operator()(const Translation & first, const Translation & second) const {
    return first.prob > second.prob;
  }
};

namespace std {
template< class T >
struct hash< std::vector< T > > {
 public:
  size_t operator()(const std::vector< T > &s) const {
    size_t result = 0;
		std::hash<T> hash;
		for(auto& word : s) {
		  result = result * 31 + hash(word);
    }
		return result;
  }
};
}

typedef unordered_map< Phrase, vector<Translation> > PhraseTable;

class PhraseTableLoader {
 public:
  PhraseTable load_phrase_table(const string & path,
                                size_t best_trans_num = 2) const;
 
 private:
  Phrase ExtractPhrase(FILE* file, int& read_symbol) const;
  void SkipDelimiter(FILE* file, int& read_symbol) const;
  double ExtractProbability(FILE* file, int& read_symbol) const;
  bool EqualPhrases(const Phrase& first, const Phrase& second) const;
};

#endif // PHRASETABLE_PHRASETABLE_H
