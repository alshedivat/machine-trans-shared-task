#ifndef PHRASETABLE_PHRASETABLE_H
#define PHRASETABLE_PHRASETABLE_H

#include <unordered_map>
#include <string>
#include <vector>

const string LANGUAGE_MODEL_PATH = "../../data/corpus/language_model.dat"
const string PHRASE_TABLE_PATH = "../../data/corpus/phrase_table.dat"
const string ENGLISH_VOCABULARY_PATH = "../../data/corpus/en.vcb"
const string ENGLISH_SENTENCES_PATH = "../../data/corpus/europarl.tok.en"
const string PHRASE_TABLE_PATH = "../../data/corpus/phrase_table.gz"

using std::unordered_map;
using std::string;
using std::vector;

typedef int Word;
typedef vector< Word > Phrase;

struct Translation {
  Phrase dest;
  double prob;
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

PhraseTable load_phrase_table(const string & path);

#endif // PHRASETABLE_PHRASETABLE_H
