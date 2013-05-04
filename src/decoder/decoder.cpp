#include "decoder.h"
#include <algorithm>
#include <limits>
#include <vector>
#include <algorithm>

using std::min;
using std::numeric_limits;
using std::vector;
using std::sort;
using std::reverse;
using std::erase;
using std::max;

Phrase Decoder::decode(const Phrase & original_sentence) {
    vector<vector<Hypothesis> > hypothesis_stacks(original_sentence.size() + 1);
    vector<vector<double> > future_costs = computeFutureCosts(original_sentence);

    Hypothesis zero_hypothesis(original_sentence);
    hypothesis_stacks[0].push_back(original_sentence);

    for (int stack_index = 0;
             stack_index <= original_sentence.size();
             ++stack_index) {
      sort(hypothesis_stacks[stack_index].begin(),
           hypothesis_stacks[stack_index].end());
      hypothesis_stacks[stack_index].reverse(hypothesis_stacks[stack_index].begin(),
                                             hypothesis_stacks[stack_index].end());

      vector<Hypothesis>::iterator iter = hypothesis_stacks[stack_index].begin();
      while ((iter != hypothesis_stacks[stack_index].end()) &&
             (iter - hypothesis_stacks[stack_index].begin() < quantity_) &&
             (iter->total_cost() - hypothesis_stacks[stack_index].begin()->total_cost() <= difference_)) {
        ++iter;
      }
      hypothesis_stack[stack_index].erase(iter, hypothesis_stacks[stack_index].end());

      for (int hypothesis_index = 0;
           hypothesis_index < hypothesis_stacks[stack_index].size();
           ++hypothesis_index) {
        Hypothesis current = hypothesis_stacks[stack_index][hypothesis_index];
        for (int phrase_begin = 0;
            phrase_begin < original_sentence.size();
            ++phrase_begin) {
          for (int phrase_end = phrase_begin + 1;
               phrase_end <= original_sentence.size();
               ++phrase_end) {

            int index = phrase_begin;
            while ((index < phrase_end) &&
                   (current.used_words[index] == false)) {
              ++index;
            }

            Phrase phrase(original_sentence.begin() + phrase_begin,
                          original_sentence.begin() + phrase_end);
            if ((index == phrase_end) && phraseInPhraseTable(phrase)) {
              Hypothesis new_hypothesis = current;
              for (int index = phrase_begin; index < phrase_end; ++index) {
                new_hypothesis.used_words[index] = true;
              }
              for (int phrase_index = 0; phrase_index < phrase_table_[phrase].size(); ++phrase_index) {
                Phrase translated_phrase = phrase_table_[phrase][phrase_index].dest;
                new_hypothesis.sentence.insert(new_hypothesis.sentence.end(), translated_phrase.begin(), translated_phrase.end());
                Phrase subsentence = Phrase(new_hypothesis.sentence.begin() +
                                              max(static_cast<int>(current.sentence.size()) - 2, 0),
                                            new_hypothesis.sentence.end());
                new_hypothesis.cost =
                      current.cost +
                      language_model_.get_probability(subsentence) +
                      alignment_model_.get_probability(phrase_begin - new_hypothesis.last_end) +
                      phrase_table_[phrase][phrase_index].source;

                new_hypothesis.future_cost = 0;
                int first = 0;
                int last = 0;
                for (int i = 0; i < new_hypothesis.used_words.size(); ++i) {
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
                  new_hypothesis.future_costs += future_costs[first][last];
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
