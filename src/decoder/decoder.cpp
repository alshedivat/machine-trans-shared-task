#include "decoder.h"
#include <algorithm>
#include <cmath>
#include <iostream>
#include <limits>
#include <vector>
#include <algorithm>

using std::accumulate;
using std::cout;
using std::endl;
using std::min;
using std::numeric_limits;
using std::vector;
using std::sort;
using std::max;

Phrase Decoder::decode(const Phrase & original_sentence) const {
  cout << "Staring decoding phrase" << endl;
  vector<vector<Hypothesis> > hypothesis_stacks(original_sentence.size() + 1);
  vector<vector<double> > future_costs = computeFutureCosts(original_sentence);
  Hypothesis zero_hypothesis(original_sentence);
  hypothesis_stacks.front().push_back(original_sentence);
  for (size_t stack_index = 0; stack_index <= original_sentence.size();
       ++stack_index) {
    cout << "Working with stack #" << stack_index << endl;
    EraseBadHypotheses(&hypothesis_stacks[stack_index]);
    for (size_t hypothesis_index = 0;
         hypothesis_index < hypothesis_stacks[stack_index].size();
         ++hypothesis_index) {
      Hypothesis current = hypothesis_stacks[stack_index][hypothesis_index];
      for (size_t phrase_begin = 0; phrase_begin < original_sentence.size();
           ++phrase_begin) {
        for (size_t phrase_end = phrase_begin + 1;
             phrase_end <= original_sentence.size();
             ++phrase_end) {
          int true_quantity = accumulate(
              current.used_words.begin() + phrase_begin,
              current.used_words.begin() + phrase_end,
              0);
          Phrase phrase(original_sentence.begin() + phrase_begin,
                        original_sentence.begin() + phrase_end);
          if ((true_quantity == 0) && phraseInPhraseTable(phrase)) {
            vector<bool> new_used_words = current.used_words;
            for (size_t index = phrase_begin; index < phrase_end; ++index) {
              new_used_words[index] = true;
            }
            for (size_t phrase_index = 0;
                 phrase_index < phrase_table_->at(phrase).size();
                 ++phrase_index) {
              Hypothesis new_hypothesis =
                  CreateNewHypothesis(phrase_begin, phrase_end, phrase_index,
                                      phrase, new_used_words, current,
                                      future_costs);
              size_t new_stack_index = phrase_end - phrase_begin + stack_index;
              hypothesis_stacks[new_stack_index].push_back(new_hypothesis);
            }
          }
        }
      }
    }
  }
  return hypothesis_stacks[original_sentence.size()][0].sentence;
}

void Decoder::EraseBadHypotheses(vector<Hypothesis> * hypothesis_vector) const {
  cout << "Erasing bad hypothesis" << endl;
  sort(hypothesis_vector->begin(), hypothesis_vector->end());
  reverse(hypothesis_vector->begin(), hypothesis_vector->end());
  vector<Hypothesis>::iterator iter = hypothesis_vector->begin();
  while ((iter != hypothesis_vector->end()) &&
         (iter - hypothesis_vector->begin() < quantity_) &&
         (hypothesis_vector->begin()->total_cost() - iter->total_cost() <=
          difference_)) {
    ++iter;
  }
  hypothesis_vector->erase(iter, hypothesis_vector->end());
}

Hypothesis Decoder::CreateNewHypothesis(
    size_t phrase_begin,
    size_t phrase_end,
    size_t phrase_index,
    const Phrase& phrase,
    const vector<bool>& new_used_words,
    const Hypothesis& current,
    const vector<vector<double> >& future_costs) const {
  cout << "Creating new hypothesis" << endl;
  Hypothesis new_hypothesis = current;
  new_hypothesis.last_end = phrase_end;
  new_hypothesis.used_words = new_used_words;
  Phrase translated_phrase = phrase_table_->at(phrase)[phrase_index].dest;
  new_hypothesis.sentence.insert(new_hypothesis.sentence.end(),
                                 translated_phrase.begin(),
                                 translated_phrase.end());
  Phrase subsentence = Phrase(new_hypothesis.sentence.begin() +
                              max(static_cast<int>(current.sentence.size()) - 2,
                                  static_cast<int>(0)),
                              new_hypothesis.sentence.end());
  double language_model_cost = 0;
  for (size_t index = 0; index + 3 <= subsentence.size(); ++index) {
    Phrase sub_subsentence = Phrase(subsentence.begin() + index, subsentence.begin() + 3);
    language_model_cost += language_model_->get_probability(sub_subsentence);
  }
  new_hypothesis.cost = current.cost +
      language_model_cost +
      alignment_model_->get_probability(static_cast<int>(phrase_begin) -
                                        current.last_end) +
      phrase_table_->at(phrase)[phrase_index].prob;
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
  return new_hypothesis;
}

vector<vector<double> > Decoder::computeFutureCosts(
    const Phrase & original_sentence) const {
  cout << "Computing future costs" << endl;
  vector<vector<double> > future_costs(
      original_sentence.size(),
      vector<double>(original_sentence.size()));
  for (size_t length = 1; length <= original_sentence.size(); ++length) {
    for (size_t start = 0; start + length <= original_sentence.size();
         ++start) {
      size_t end = start + length - 1;
      future_costs[start][end] = numeric_limits<double>::min();
      Phrase phrase_part(original_sentence.begin() + start,
                         original_sentence.begin() + end);
      if (phraseInPhraseTable(phrase_part)) {
        future_costs[start][end] =
            getMostProbableCost(phrase_part) +
            language_model_->get_probability(phrase_part);
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
    return phrase_table_->find(phrase) != phrase_table_->end();
}

double Decoder::getMostProbableCost(const Phrase& phrase) const {
    return log(phrase_table_->at(phrase).begin()->prob);
}
