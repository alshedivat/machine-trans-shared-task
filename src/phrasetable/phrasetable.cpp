
#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>
#include "phrasetable.h"
#include <iostream>

using std::cout;
using std::endl;

PhraseTable load_phrase_table(const string& path, size_t best_trans_num) {
  cout << "Loading phrase table" << endl;
  PhraseTable phrase_table;
  std::ifstream phrase_table_file(path.c_str());
  string line;
  Phrase prev_fr_phrase;
  std::getline(phrase_table_file, line); // skip first line with headers
  int index = 0;
  while(std::getline(phrase_table_file, line)) {
    ++index;
    if (index % 5000000 == 0) {
      cout << "Read " << index << " lines" << endl;
    }
    Phrase en_phrase, fr_phrase;
    int number_phrase = 0;
    std::stringstream sstr(line);
    string token;
    while(number_phrase < 2) {
      sstr >> token;
      if (token == "|||")
        number_phrase++;
      else
        if (number_phrase == 0)
          fr_phrase.push_back(atoi(token.c_str()));
        else
          en_phrase.push_back(atoi(token.c_str()));
    }
    if (prev_fr_phrase.size() == 0)
      prev_fr_phrase = fr_phrase;
    if (fr_phrase != prev_fr_phrase) {
      std::sort(phrase_table[prev_fr_phrase].begin(),
                phrase_table[prev_fr_phrase].end(),
                pred());
      if (phrase_table[prev_fr_phrase].size() > best_trans_num) {
        auto start = phrase_table[prev_fr_phrase].begin() + best_trans_num;
        auto end   = phrase_table[prev_fr_phrase].end();
        phrase_table[prev_fr_phrase].erase(start, end);
      }
    }

    sstr >> token;
    double prob = atof(token.c_str());
    Translation trans(en_phrase, prob);
    if (!phrase_table.count(fr_phrase))
      phrase_table[fr_phrase] = vector<Translation>();
    phrase_table[fr_phrase].push_back(trans);
    prev_fr_phrase = fr_phrase;
  }
  cout << "Phrase table loaded" << endl;
  return phrase_table;
}
