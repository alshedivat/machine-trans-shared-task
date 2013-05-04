#ifndef LANGUAGEMODEL_H
#define LANGUAGEMODEL_H

#include "../phrasetable/phrasetable.h"

class LanguageModel {
 public:
	virtual double get_probability(const Phrase& sentance) = 0;
	virtual ~LanguageModel() { }
};

/// sentances - corpus, wordsCount - dictionary size, N - parameter in N-gram model, alpha - smoothing
LanguageModel* learnNgramLanguageModel(const std::vector< Phrase >& sentances, const int wordsCount, const int N = 3, const double alpha = 0.00017);

#endif // LANGUAGEMODEL_H
