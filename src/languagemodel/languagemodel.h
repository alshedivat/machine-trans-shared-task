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

	template< class T >
	NgramLanguageModel(T&& map, T&& context, size_t N, double alpha, double nu)
		: seenPhrases_(std::forward< T >(map))
		, seenContext_(std::forward< T >(context))
		, N_(N)
		, alpha_(alpha)
		, nu_(nu)
	{
	}

	double get_probability(const Phrase& sentence) const {
		if (sentence.size() < 1)
			throw std::runtime_error("wrong phrase length");
		Phrase phrase(N_ - 1, 0);
		phrase.insert(phrase.end(), sentence.begin(), sentence.end());
		double language_model_cost = 0;
		for (size_t index = 0; index + 3 <= phrase.size(); ++index) {
			Phrase subphrase = Phrase(phrase.begin() + index, phrase.begin() + index + 3);

			double num = get_count(subphrase);
			double denum = get_context_count(subphrase);
			language_model_cost += log(num + alpha_) - log(denum + nu_ * alpha_);
		}
		return language_model_cost;
	}
	
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
NgramLanguageModel learn_ngram_language_model(const std::vector< Phrase >& sentences, const int wordsCount, const size_t N = 3, const double alpha = 0.00017);

NgramLanguageModel load_ngram_language_model(const std::string& path);


#endif // LANGUAGEMODEL_H
