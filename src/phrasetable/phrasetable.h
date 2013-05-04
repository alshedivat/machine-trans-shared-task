#ifndef PHRASETABLE_PHRASETABLE_H
#define PHRASETABLE_PHRASETABLE_H

#include <unordered_map>
#include <string>
#include <vector>

using std::unordered_map;
using std::string;
using std::vector;

const string LANGUAGE_MODEL_PATH = "../data/corpus/language_model.dat";
const string ENGLISH_VOCABULARY_PATH = "../data/corpus/en.vcb";
const string FRENCH_VOCABULARY_PATH = "../data/corpus/fr.vcb";
const string ENGLISH_SENTENCES_PATH = "../data/corpus/europarl.lowercased.en";
const string PHRASE_TABLE_PATH = "../data/corpus/phrase_table.gz";

typedef size_t Word;
typedef vector< Word > Phrase;

struct Translation {
  Translation(Phrase phrase, double probability)
      : dest(phrase), prob(probability)
  {}

  Phrase dest;
  double prob;
};

struct pred {
  bool operator()(Translation const & a, Translation const & b) const {
    return a.prob < b.prob;
  }
};

namespace std {
template< class T >
struct hash< std::vector< T > > {
public:
    size_t operator()(const std::vector< T > &s) const
    {
		size_t result = 0;
		std::hash<T> hash;
		for(auto& word : s)
			result = result * 31 + hash(word);
		return result;
    }
};
}

typedef unordered_map< Phrase, vector<Translation> > PhraseTable;

PhraseTable load_phrase_table(const string & path, size_t best_trans_num = 2);

#endif // PHRASETABLE_PHRASETABLE_H
