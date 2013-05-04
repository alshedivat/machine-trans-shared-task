#ifndef PHRASETABLE_PHRASETABLE_H
#define PHRASETABLE_PHRASETABLE_H

#include <unordered_map>
#include <string>
#include <vector>

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
