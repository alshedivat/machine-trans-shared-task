#ifndef PHRASETABLE_H
#define PHRASETABLE_H

#include <unordered_map>
#include <string>
#include <vector>

using std::unordered_map;
using std::string;
using std::vector;

typedef vector<int> Phrase;

struct Translation {
  Phrase dest;
  double prob;
};

typedef unordered_map<vector<int>, vector<Translation> > PhraseTable;

PhraseTable load_phrase_table(const string & path);

#endif // PHRASETABLE_H
