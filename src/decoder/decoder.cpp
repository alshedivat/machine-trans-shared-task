#include "decoder.h"
#include <algorithm>
#include <cmath>
#include <iostream>
#include <limits>
#include <vector>
#include <algorithm>
#include <numeric>

using std::accumulate;
using std::cout;
using std::endl;
using std::min;
using std::numeric_limits;
using std::vector;
using std::sort;
using std::max;

Phrase Decoder::decode(const Phrase & original_sentence,
                       size_t max_phrase_length,
                       size_t max_swap_distance) const {
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
      size_t first_unused_word = 0;
      while ( (first_unused_word < current.used_words.size()) &&
              (current.used_words[first_unused_word] == true)) {
          ++first_unused_word;
        }
      for (size_t phrase_begin = first_unused_word;
           phrase_begin < min(first_unused_word + max_swap_distance, original_sentence.size());
           ++phrase_begin) {
        for (size_t phrase_end = phrase_begin + 1;
             phrase_end <= min(phrase_begin + max_phrase_length, original_sentence.size());
             ++phrase_end) {
          int true_quantity = accumulate(
              current.used_words.begin() + phrase_begin,
              current.used_words.begin() + phrase_end,
              0);
          Phrase phrase(original_sentence.begin() + phrase_begin,
                        original_sentence.begin() + phrase_end);
          if (true_quantity == 0) {
            vector<Translation> translations = getTranslations(phrase);
            vector<bool> new_used_words = current.used_words;
            for (size_t index = phrase_begin; index < phrase_end; ++index) {
              new_used_words[index] = true;
            }
            for (size_t phrase_index = 0;
                 phrase_index < translations.size();
                 ++phrase_index) {
              Hypothesis new_hypothesis =
                  CreateNewHypothesis(phrase_begin, phrase_end, phrase_index,
                                      phrase, new_used_words, current,
                                      translations, future_costs);
              size_t new_stack_index = phrase_end - phrase_begin + stack_index;
              hypothesis_stacks[new_stack_index].push_back(new_hypothesis);
            }
          }
        }
      }
    }
  }
  size_t index = original_sentence.size();
  while (hypothesis_stacks[index].size() == 0) {
    --index;
  }
  return hypothesis_stacks[index][0].sentence;
}

vector<Translation> Decoder::getTranslations(const Phrase & phrase) const {
  if (phrase_table_->count(phrase) != 0) {
    return phrase_table_->at(phrase);
  } else if (phrase.size() == 1) {
    return vector<Translation>(1, Translation(phrase, 1));
  } else {
    return vector<Translation>();
  }
}

void Decoder::EraseBadHypotheses(vector<Hypothesis> * hypothesis_vector) const {
  sort(hypothesis_vector->rbegin(), hypothesis_vector->rend());
  vector<Hypothesis>::iterator iter = hypothesis_vector->begin();
  while ((iter != hypothesis_vector->end()) &&
         (iter - hypothesis_vector->begin() < quantity_) &&
         (hypothesis_vector->begin()->total_cost() - iter->total_cost() <=
          difference_)) {
    ++iter;
  }
  hypothesis_vector->erase(iter, hypothesis_vector->end());
  hypothesis_vector->shrink_to_fit();
}

Hypothesis Decoder::CreateNewHypothesis(
    size_t phrase_begin,
    size_t phrase_end,
    size_t phrase_index,
    const Phrase& phrase,
    const vector<bool>& new_used_words,
    const Hypothesis& current,
    const vector<Translation> & translations,
    const vector<vector<double> >& future_costs) const {
  Hypothesis new_hypothesis = current;
  new_hypothesis.last_end = phrase_end;
  new_hypothesis.used_words = new_used_words;
  Phrase translated_phrase = translations[phrase_index].dest;
  new_hypothesis.sentence.insert(new_hypothesis.sentence.end(),
                                 translated_phrase.begin(),
                                 translated_phrase.end());

  new_hypothesis.language_model_cost = language_model_->get_probability(new_hypothesis.sentence);
  new_hypothesis.cost = current.cost +
      alignment_model_->get_probability(static_cast<int>(phrase_begin) -
                                        static_cast<int>(current.last_end)) +
      log(translations[phrase_index].prob);
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
	      Translation tr = getMostProbableTranslation(phrase_part);
        future_costs[start][end] =
            log(tr.prob) +
            language_model_->get_probability(tr.dest);
      }
      for (size_t i = start; i < end; ++i) {
        double new_cost = future_costs[start][i] + future_costs[i + 1][end];
        future_costs[start][end] = max(future_costs[start][end], new_cost);
      }
    }
  }
  return future_costs;
}

bool Decoder::phraseInPhraseTable(const Phrase& phrase) const {
    return phrase_table_->find(phrase) != phrase_table_->end();
}

Translation Decoder::getMostProbableTranslation(const Phrase& phrase) const {
    return *phrase_table_->at(phrase).begin();
}
