#include "decoder.h"
#include <algorithm>
#include <limits>
#include <vector>

using std::min;
using std::numeric_limits;
using std::vector;

Phrase Decoder::decode(const Phrase & original_sentence) {
    vector<vector<Hypothesis> > hypothesis_stack(original_sentence.size());
    vector<vector<double> > future_costs = computeFutureCosts(original_sentence);
    return original_sentence;
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