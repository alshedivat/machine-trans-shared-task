#ifndef LANGUAGEMODEL_H
#define LANGUAGEMODEL_H

#include "../phrasetable/phrasetable.h"
#include <stdexcept>
#include <unordered_map>
#include <math.h>

class NgramLanguageModel
{
public:

	typedef std::unordered_map< Phrase, double > Phrase2Stat;

	NgramLanguageModel() {
	}

	template< class T >
	NgramLanguageModel(T&& map, T&& context, size_t N, double alpha, double nu)
		: seenPhrases_(std::forward< T >(map))
		, seenContext_(std::forward< T >(context))
		, N_(N)
		, alpha_(alpha)
		, nu_(nu)
	{
	}

	/// returns probability of whole sentence
	double get_probability(const Phrase& sentence) const;

	size_t get_length() const {
		return N_;
	}

	void save(const std::string& path) const;

	bool operator ==(const NgramLanguageModel& other) const;

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
NgramLanguageModel learn_ngram_language_model(const std::vector< Phrase >& sentences, const size_t wordsCount, const size_t N = 3, const double alpha = 0.00017);

NgramLanguageModel load_ngram_language_model(const std::string& path);


#endif // LANGUAGEMODEL_H
