#ifndef LANGUAGEMODEL_H
#define LANGUAGEMODEL_H

#include "../phrasetable/phrasetable.h"
#include <stdexcept>
#include <unordered_map>

class NgramLanguageModel
{
public:

	typedef std::unordered_map< Phrase, double > Phrase2Stat;

	template< class T >
	NgramLanguageModel(T&& map, T&& context, size_t N, double alpha, double nu)
		: seenPhrases_(std::forward< T >(map))
		, seenContext_(std::forward< T >(context))
		, N_(N)
		, alpha_(alpha)
		, nu_(nu)
	{
	}

	double get_probability(const Phrase& phrase) {
		if (phrase.size() < 1)
			throw std::runtime_error("wrong phrase length");
		double num = get_count(phrase);
		double denum = get_context_count(phrase);
		return (num + alpha_) / (denum + nu_ * alpha_);
	}

private:

	double get_count(const Phrase& phrase) const;

	double get_context_count(Phrase phrase) const;

	Phrase2Stat seenPhrases_;
	Phrase2Stat seenContext_;
	size_t N_;
	double alpha_;
	double nu_;
};

typedef NgramLanguageModel LanguageModel;

/// sentences - corpus, wordsCount - dictionary size, N - parameter in N-gram model, alpha - smoothing
NgramLanguageModel learn_ngram_language_model(const std::vector< Phrase >& sentences, const int wordsCount, const size_t N = 3, const double alpha = 0.00017);

#endif // LANGUAGEMODEL_H
