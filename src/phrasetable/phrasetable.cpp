#include <algorithm>
#include <fstream>
#include <sstream>
#include "phrasetable.h"

PhraseTable load_phrase_table(const string& path, size_t best_trans_num) {
  PhraseTable phrase_table;
  std::ifstream phrase_table_file(path.c_str());
  string line;
  Phrase prev_fr_phrase;
  while(std::getline(phrase_table_file, line)) {
    Phrase en_phrase, fr_phrase;
    bool first_phrase = true;
    std::stringstream sstr(line);
    string token;
    while(sstr >> token) {
      if (token == "|||")
        first_phrase = false;
      else
        if (first_phrase)
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
      auto start = phrase_table[prev_fr_phrase].begin() + best_trans_num;
      auto end   = phrase_table[prev_fr_phrase].end();
      phrase_table[prev_fr_phrase].erase(start, end);
    }

    double prob = atof(token.c_str());
    Translation trans(en_phrase, prob);
    if (!phrase_table.count(fr_phrase))
      phrase_table[fr_phrase] = vector<Translation>();
    phrase_table[fr_phrase].push_back(trans);
  }

  return phrase_table;
}
