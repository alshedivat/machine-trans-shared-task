#include "languagemodel.h"
#include "languagemodel.h"

#include <unordered_map>
#include <string>
#include <fstream>
#include <sstream>
#include <memory>

int getId(std::unordered_map< std::string, int >& dictionary, const std::string& word) {
	auto it = dictionary.find(word);
	if (it == dictionary.end()) {
		int id = dictionary.size();
		return dictionary[word] = id;
	}
	return it->second;
}

int main() {
	std::unordered_map< std::string, int > dictionary;
	//std::ifstream input("../data/corpus/europarl.lowercased.en", std::ios::in);
	int phrases[][3] = {
		{ 0, 1, 2 },
		{ 0, 2, 3 },
		{ 2, 3, 1 },
	};
	/*for (std::string line; std::getline(input, line); ) {
		std::istringstream iss(line);
		Phrase phrase;
		for (std::string word; iss >> word; )
			phrase.push_back(getId(dictionary, word));
		phrases.push_back(phrase);
		if (phrases.size() == 1000)
			break;
	}*/
	std::vector< Phrase > phs;
	for (int i = 0; i < 3; ++i)
		phs.push_back(Phrase(phrases[i], phrases[i+1]));
	LanguageModel model(learn_ngram_language_model(phs, 4, 2, 0.01));

	for (int i = 0; i < 4; ++i) {

		for (int j=  0; j < 4; ++j) {
			Phrase phrase;
			phrase.push_back(i);
			phrase.push_back(j);
			printf("%d %d %lf\n", i, j, model.get_probability(phrase));
		}
		Phrase phrase;
		phrase.push_back(i);
			printf("%d %lf\n", i, model.get_probability(phrase));
	}

	model.save("lmodel.bin");
	if (!(model == load_ngram_language_model("lmodel.bin")))
		throw 42;
}
