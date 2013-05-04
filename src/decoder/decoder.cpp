#include "decoder.h"
#include <algorithm>
#include <limits>
#include <vector>
#include <algorithm>

using std::min;
using std::numeric_limits;
using std::vector;
using std::sort;
using std::max;

Phrase Decoder::decode(const Phrase & original_sentence) const {
    vector<vector<Hypothesis> > hypothesis_stacks(original_sentence.size() + 1);
    vector<vector<double> > future_costs = computeFutureCosts(original_sentence);

    Hypothesis zero_hypothesis(original_sentence);
    hypothesis_stacks.front().push_back(original_sentence);

    for (size_t stack_index = 0; stack_index <= original_sentence.size();
         ++stack_index) {
      EraseBadHypotheses(&hypothesis_stacks[stack_index]);

      for (size_t hypothesis_index = 0;
           hypothesis_index < hypothesis_stacks[stack_index].size();
           ++hypothesis_index) {
        Hypothesis current = hypothesis_stacks[stack_index][hypothesis_index];
        for (size_t phrase_begin = 0;
            phrase_begin < original_sentence.size();
            ++phrase_begin) {
          for (size_t phrase_end = phrase_begin + 1;
               phrase_end <= original_sentence.size();
               ++phrase_end) {

            size_t index = phrase_begin;
            while ((index < phrase_end) &&
                   (current.used_words[index] == false)) {
              ++index;
            }

            Phrase phrase(original_sentence.begin() + phrase_begin,
                          original_sentence.begin() + phrase_end);
            if ((index == phrase_end) && phraseInPhraseTable(phrase)) {
              Hypothesis new_hypothesis = current;
              for (size_t index = phrase_begin; index < phrase_end; ++index) {
                new_hypothesis.used_words[index] = true;
              }
              for (size_t phrase_index = 0; phrase_index < phrase_table_.at(phrase).size(); ++phrase_index) {
                Phrase translated_phrase = phrase_table_.at(phrase)[phrase_index].dest;
                new_hypothesis.sentence.insert(new_hypothesis.sentence.end(), translated_phrase.begin(), translated_phrase.end());
                Phrase subsentence = Phrase(new_hypothesis.sentence.begin() +
                                              max(static_cast<int>(current.sentence.size()) - 2, 0),
                                            new_hypothesis.sentence.end());
                new_hypothesis.cost =
                      current.cost +
                      language_model_.get_probability(subsentence) +
                      alignment_model_.get_probability(static_cast<int>(phrase_begin) - new_hypothesis.last_end) +
                      phrase_table_.at(phrase)[phrase_index].prob;

                new_hypothesis.future_cost = 0;
                size_t first = 0;
                size_t last = 0;
                for (size_t i = 0; i < new_hypothesis.used_words.size(); ++i) {
                  if (new_hypothesis.used_words[i] == false) {
                    ++last;
                  } else {
                    if (new_hypothesis.used_words[last] == false) {
                      new_hypothesis.future_cost += future_costs[first][last];
                    }
                    last = i;
                    first = i + 1;
                  }
                }
                if (new_hypothesis.used_words[last] == false) {
                  new_hypothesis.future_cost += future_costs[first][last];
                }
                new_hypothesis.last_end = phrase_end;

                hypothesis_stacks[phrase_end - phrase_begin].push_back(new_hypothesis);
              }
            }
          }
        }
      }
    }
    return hypothesis_stacks[original_sentence.size()][0].sentence;
}

void Decoder::EraseBadHypotheses(vector<Hypothesis> * hypothesis_vector) const {
    sort(hypothesis_vector->begin(), hypothesis_vector->end());
    reverse(hypothesis_vector->begin(), hypothesis_vector->end());
    vector<Hypothesis>::iterator iter = hypothesis_vector->begin();
    while ((iter != hypothesis_vector->end()) &&
           (iter - hypothesis_vector->begin() < quantity_) &&
           (hypothesis_vector->begin()->total_cost() - iter->total_cost() <= difference_)) {
        ++iter;
    }
    hypothesis_vector->erase(iter, hypothesis_vector->end());
}

vector<vector<double> > Decoder::computeFutureCosts(const Phrase & original_sentence) const {
    vector<vector<double> > future_costs(original_sentence.size(),
                                         vector<double>(original_sentence.size()));
    for (size_t length = 1; length <= original_sentence.size(); ++length) {
        for (size_t start = 0; start < original_sentence.size(); ++start) {
            size_t end = start + length;
            future_costs[start][end] = numeric_limits<double>::min();
            Phrase phrase_part(original_sentence.begin() + start,
                               original_sentence.begin() + end);
            if (phraseInPhraseTable(phrase_part)) {
                future_costs[start][end] = getMostProbableCost(phrase_part);
            }
            for (size_t i = start; i < end; ++i) {
                double new_cost = future_costs[start][i] + future_costs[i + 1][end];
                future_costs[start][end] = min(future_costs[start][end], new_cost);
            }
        }
    }
    return future_costs;
}

bool Decoder::phraseInPhraseTable(const Phrase& phrase) const {
    return phrase_table_.find(phrase) != phrase_table_.end();
}

double Decoder::getMostProbableCost(const Phrase& phrase) const {
    return phrase_table_.at(phrase).begin()->prob;
}
