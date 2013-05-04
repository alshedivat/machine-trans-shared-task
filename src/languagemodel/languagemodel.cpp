#ifndef LANGUAGEMODEL_LANGUAGEMODEL_H
#define LANGUAGEMODEL_LANGUAGEMODEL_H

#include <unordered_map>
#include <fstream>

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

template< class T >
static void write(const T& t, FILE* output) {
	fwrite(&t, sizeof(t), 1, output);
}

static void writePhraseMap(const NgramLanguageModel::Phrase2Stat& map, FILE* output) {
	write(map.size(), output);
	for(auto& phraseStat : map) {
		auto& phrase = phraseStat.first;
		write(phrase.size(), output);
		for (Word w : phrase)
			write(w, output);
		write(phraseStat.second, output);
	}
}

template< class T >
static T read(FILE* input) {
	T result;
	fread(&result, sizeof(result), 1, input);
	return result;
}

static NgramLanguageModel::Phrase2Stat readPhraseMap(FILE* input) {
	NgramLanguageModel::Phrase2Stat result;
	const size_t count = read< size_t >(input);
	for (size_t i = 0; i < count; ++i) {
		const size_t phraseLength = read< size_t >(input);
		Phrase phrase;
		phrase.reserve(phraseLength);
		for (size_t j = 0; j < phraseLength; ++j) {
			phrase.push_back(read< Word >(input));
		}
		result[phrase] = read< NgramLanguageModel::Phrase2Stat::mapped_type >(input);
	}
	return result;
}

void NgramLanguageModel::save(const std::string& path) const {
	FILE* output = fopen(path.c_str(), "wb");
	write(N_, output);
	write(alpha_, output);
	write(nu_, output);
	writePhraseMap(seenPhrases_, output);
	writePhraseMap(seenContext_, output);
	fclose(output);
}

bool NgramLanguageModel::operator ==(const NgramLanguageModel& other) const {
	return seenContext_ == other.seenContext_ 
		&& seenPhrases_ == other.seenPhrases_
		&& N_ == other.N_
		&& alpha_ == other.alpha_
		&& nu_ == other.nu_;
}


NgramLanguageModel load_ngram_language_model(const std::string& path) {
	FILE* input = fopen(path.c_str(), "rb");
	size_t N = read< size_t >(input);
	double alpha = read< double >(input);
	double nu = read< double >(input);
	auto seenPhrases = readPhraseMap(input);
	auto seenContext = readPhraseMap(input);
	fclose(input);
	return NgramLanguageModel(std::move(seenPhrases), std::move(seenContext), N, alpha, nu);
}


NgramLanguageModel learn_ngram_language_model(const std::vector< Phrase >& sentences, const int wordsCount, const size_t N, const double alpha) {
	const double nu = wordsCount;
	NgramLanguageModel::Phrase2Stat stat;
	NgramLanguageModel::Phrase2Stat statContext;
	for(const Phrase& sentance : sentences) {
		Phrase phrase;
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
