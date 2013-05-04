#ifndef PHRASETABLE_H
#define PHRASETABLE_H

#include <vector>

using namespace std;

typedef vector<int> Phrase;

struct Translation {
  Phrase dest;
  double prob;
};

typedef map<vector<int>, vector<Translation> > PhraseTable;

PhraseTable load_phrase_table(const string & path);

#endif // PHRASETABLE_H
