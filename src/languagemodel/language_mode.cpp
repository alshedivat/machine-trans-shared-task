#include "LanguageModel.h"

#include <unordered_map>
#include <boost/foreach.hpp>

template< class Map >
static const typename Map::mapped_type& mapAtOrDefault(const Map& map, const typename Map::key_type& key, const typename Map::mapped_type& default) {
	auto iterator = map.find(key);
	if (iterator == map.end())
		return default;
	return iterator->second;
}

 
namespace std {
template< class T >
struct hash< std::vector< T > > {
public:
    size_t operator()(const std::vector< T > &s) const 
    {
		size_t result = 0;
		std::hash<T> hash;
		BOOST_FOREACH(auto& word, s)
			result = result * 31 + hash(word);
		return result;
    }
};
}

namespace {

class NgramLanguageModel : public LanguageModel
{
public:

	typedef std::unordered_map< Phrase, double > Phrase2Stat;

	template< class T >
	NgramLanguageModel(T&& map, T&& context, int N, double alpha, double nu) 
		: seenPhrases_(std::forward< T >(map)),
		seenContext_(std::forward< T >(context)),
		N_(N),
		alpha_(alpha),
		nu_(nu)
	{
	}

	double get_probability(const Phrase& phrase) {
		if (phrase.size() < 1)
			throw std::runtime_error("wrong phrase length");
		double num = getCount(phrase);
		double denum = getContextCount(phrase);
		return (num + alpha_) / (denum + nu_ * alpha_);
	}

private:

	double getCount(const Phrase& phrase) const {
		return mapAtOrDefault(seenPhrases_, phrase, 0);
	}

	double getContextCount(Phrase phrase) const {
		phrase.pop_back();
		return mapAtOrDefault(seenContext_, phrase, 0);
	}

	Phrase2Stat seenPhrases_;
	Phrase2Stat seenContext_;
	int N_;
	double alpha_;
	double nu_;
};

}

LanguageModel* learnNgramLanguageModel(const std::vector< Phrase >& sentances, const int wordsCount, const int N, const double alpha) {
	const double nu = wordsCount;
	NgramLanguageModel::Phrase2Stat stat;
	NgramLanguageModel::Phrase2Stat statContext;
	BOOST_FOREACH(const Phrase& sentance, sentances) {
		std::vector< int > phrase;
		stat[phrase]++;
		BOOST_FOREACH(Word word, sentance) {
			if (phrase.size() < N) {
				statContext[phrase]++;
			} else {
				std::copy(phrase.begin() + 1, phrase.end(), phrase.begin());
				phrase.pop_back();
				statContext[phrase]++;
			}
			phrase.push_back(word);
			stat[phrase]++;
		}
	}
	return new NgramLanguageModel(std::move(stat), std::move(statContext), N, alpha, nu);
}

