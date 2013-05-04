#include "decoder.h"
#include <cfloat>
#include <vector>

using std::vector;

Phrase Decoder::decode(const Phrase & original_sentence) {
    vector<vector<Hypothesis> > hypothesis_stack(original_sentence.size());
    vector<vector<double> > future_costs = computeFutureCosts(original_sentence);
}

vector<vector<double> > Decoder::computeFutureCosts(const Phrase & original_sentence) const {
    vector<vector<double> > future_costs(original_sentence.size(),
                                         vector<double>(original_sentence.size()));
    for (size_t length = 1; length <= original_sentence.size(); ++length) {
        for (size_t start = 0; start < original_sentence.size(); ++start) {
            size_t end = start + length;
            future_costs[start][end] = DBL_MAX;
        }
    }
    return future_costs;
}