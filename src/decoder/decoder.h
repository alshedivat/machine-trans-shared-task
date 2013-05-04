#ifndef DECODER_H
#define DECODER_H

#include <vector>

#include "../phrasetable/phrasetable.h"
#include "../languagemodel/languagemodel.h"
#include "../alignmentmodel/alignmentmodel.h"

using std::vector;

struct Hypothesis {
  Phrase sentence;
  vector<bool> used_words;
  double cost, future_cost;
  int last_end;

  Hypothesis()
    : sentence(0)
    , used_words(0)
    , cost(0)
    , future_cost(0)
    , last_end(0) {}

  Hypothesis(const Phrase & original_sentence)
    : sentence(0)
    , used_words(original_sentence.size())
    , cost(0)
    , future_cost(0)
    , last_end(0) {}

  bool operator < (const Hypothesis & left) {
    return cost + future_cost < left.cost + left.future_cost;
  }
};

class Decoder {
 public:
  Decoder(const LanguageModel & language_model,
          const AlignmentModel & alignment_model,
          const PhraseTable & phrase_table,
          int difference, int quantity)
    : difference_(difference)
    , quantity_(quantity)
    , language_model_(language_model)
    , alignment_model_(alignment_model)
    , phrase_table_(phrase_table) {}

  Phrase decode(const Phrase & original_sentence);

 private:
  int difference_;
  int quantity_;
  LanguageModel language_model_;
  AlignmentModel alignment_model_;
  PhraseTable phrase_table_;
};

#endif // DECODER_H
