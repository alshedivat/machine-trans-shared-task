#ifndef LANGUAGEMODEL_LANGUAGEMODEL_H
#define LANGUAGEMODEL_LANGUAGEMODEL_H

#include <unordered_map>

#include "languagemodel.h"

template< class Map >
static const typename Map::mapped_type& mapAtOrDefault(const Map& map, const typename Map::key_type& key, const typename Map::mapped_type& defaultResult) {
	auto iterator = map.find(key);
	if (iterator == map.end())
		return defaultResult;
	return iterator->second;
}

double NgramLanguageModel::get_count(const Phrase& phrase) const {
	return mapAtOrDefault(seenPhrases_, phrase, 0);
}

double NgramLanguageModel::get_context_count(Phrase phrase) const {
	phrase.pop_back();
	return mapAtOrDefault(seenContext_, phrase, 0);
}

NgramLanguageModel learn_ngram_language_model(const std::vector< Phrase >& sentences, const int wordsCount, const int N, const double alpha) {
	const double nu = wordsCount;
	NgramLanguageModel::Phrase2Stat stat;
	NgramLanguageModel::Phrase2Stat statContext;
	for(const Phrase& sentance : sentences) {
		std::vector< int > phrase;
		stat[phrase]++;
		for(Word word : sentance) {
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
	return NgramLanguageModel(std::move(stat), std::move(statContext), N, alpha, nu);
}

#endif // LANGUAGEMODEL_LANGUAGEMODEL_H
