#include "decoder.h"
#include <algorithm>
#include <limits>
#include <vector>
#include <algorithm>
#include <iostream>
#include <exception>

using std::cerr;
using std::cout;
using std::endl;
using std::min;
using std::numeric_limits;
using std::vector;
using std::sort;
using std::max;

Phrase Decoder::decode(const Phrase & original_sentence) const {
    vector<vector<Hypothesis> > hypothesis_stacks(original_sentence.size() + 1);
    cout << "Start computing Future costs" << endl;
    vector<vector<double> > future_costs = computeFutureCosts(original_sentence);
    cout << "Future costs computed" << endl;
    Hypothesis zero_hypothesis(original_sentence);
    hypothesis_stacks.front().push_back(original_sentence);

    for (size_t stack_index = 0; stack_index <= original_sentence.size();
         ++stack_index) {
      cout << "Stack: " << stack_index << endl;
      EraseBadHypotheses(&hypothesis_stacks[stack_index]);
      cout << "Cleared stack" << endl;

      for (size_t hypothesis_index = 0;
           hypothesis_index < hypothesis_stacks[stack_index].size();
           ++hypothesis_index) {
        cout << "Hypothesis: " << hypothesis_index << endl;
        Hypothesis current = hypothesis_stacks[stack_index][hypothesis_index];
        cout << "Got hypothesis" << endl;
        for (size_t phrase_begin = 0;
            phrase_begin < original_sentence.size();
            ++phrase_begin) {
          for (size_t phrase_end = phrase_begin + 1;
               phrase_end <= original_sentence.size();
               ++phrase_end) {
            cout << "Phrase: " << phrase_begin << " - " << phrase_end << endl;
            int true_quantity = std::accumulate(current.used_words.begin() + phrase_begin,
                                                current.used_words.begin() + phrase_end,
                                                0);
            cout << "Computed true_quantity: " << true_quantity << endl;
            Phrase phrase(original_sentence.begin() + phrase_begin,
                          original_sentence.begin() + phrase_end);
            for (size_t ii = 0; ii < phrase.size(); ++ii) {
              cout << "Phrase code: " << phrase[ii] << " ";
            }
            cout << endl;
            if ((true_quantity == 0) && phraseInPhraseTable(phrase)) {
              cout << "Is good phrase" << endl;
              vector<bool> new_used_words = current.used_words;
              for (size_t index = phrase_begin; index < phrase_end; ++index) {
                new_used_words[index] = true;
              }
              for (size_t phrase_index = 0; phrase_index < phrase_table_.at(phrase).size(); ++phrase_index) {
                cout << "Trying create hypothesis" << endl;
                Hypothesis new_hypothesis = CreateNewHypothesis(phrase_begin, phrase_end, phrase_index, phrase,
                                                                new_used_words, current, future_costs);
                hypothesis_stacks[phrase_end - phrase_begin + stack_index].push_back(new_hypothesis);
                cout << "Added new hypothesis";
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

Hypothesis Decoder::CreateNewHypothesis(size_t phrase_begin, size_t phrase_end, size_t phrase_index,
                                        const Phrase& phrase, const vector<bool>& new_used_words,
                                        const Hypothesis& current,
                                        const vector<vector<double> >& future_costs) const {
    Hypothesis new_hypothesis = current;
    new_hypothesis.last_end = phrase_end;
    new_hypothesis.used_words = new_used_words;
    Phrase translated_phrase = phrase_table_.at(phrase)[phrase_index].dest;
    cout << phrase.size() << endl;
    cout << phrase_table_.at(phrase)[phrase_index].dest.size() << " " << phrase_table_.at(phrase)[phrase_index].prob << endl;
    cout << translated_phrase.size() << endl;
    new_hypothesis.sentence.insert(new_hypothesis.sentence.end(),
                                   translated_phrase.begin(),
                                   translated_phrase.end());
    cout << new_hypothesis.sentence.size() << endl;
    Phrase subsentence = Phrase(new_hypothesis.sentence.begin() +
                                max(static_cast<int>(current.sentence.size()) - 2, static_cast<int>(0)),
                                new_hypothesis.sentence.end());
    cout << subsentence.size() << endl;
    new_hypothesis.cost = current.cost +
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
    return new_hypothesis;
}

vector<vector<double> > Decoder::computeFutureCosts(const Phrase & original_sentence) const {
    vector<vector<double> > future_costs(original_sentence.size(),
                                         vector<double>(original_sentence.size()));
    for (size_t length = 1; length <= original_sentence.size(); ++length) {
        for (size_t start = 0; start + length <= original_sentence.size(); ++start) {
            size_t end = start + length - 1;
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
