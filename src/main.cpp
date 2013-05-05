#include <fstream>
#include <iostream>
#include <stdexcept>
#include "alignmentmodel/alignmentmodel.h"
#include "converter/converter.h"
#include "decoder/decoder.h"
#include "languagemodel/languagemodel.h"
#include "phrasetable/phrasetable.h"
#include "programoptionsparser/programoptionsparser.h"

using std::cerr;
using std::cin;
using std::cout;
using std::getline;
using std::endl;
using std::exception;
using std::ifstream;
using std::ofstream;
using std::runtime_error;
using std::string;

LanguageModel loadLanguageModel(const Converter& converter,
                                const string& language_model_path,
                                const string& english_sentences_path) {
    ifstream file(language_model_path.data());
    if (file) {
		file.close();
		return load_ngram_language_model(language_model_path.data());
	}
	file.open(english_sentences_path.data());
	if (!file) {
		throw runtime_error("Failed to load english sentences");
	}
	string sentence;
	vector<Phrase> sentences;
	cout << "Reading english corpus" << endl;
	while (getline(file, sentence)) {
		Phrase sent = converter.ToIndex(sentence);
		sentences.push_back(sent);
	}
	file.close();
    return learn_ngram_language_model(sentences, converter.DictSize(), 3);
}

int main(int argc, char** argv) {
    try {
        ProgramOptionsParser program_options_parser;
        program_options_parser.Parse(argc, argv);
        Converter english_converter(
            program_options_parser.english_vocabulary_path());
        Converter french_converter(
            program_options_parser.french_vocabulary_path());
        LanguageModel language_model = loadLanguageModel(
            english_converter,
            program_options_parser.language_model_path(),
            program_options_parser.english_sentences_path());
        AlignmentModel alignment_model;
        PhraseTable phrase_table = load_phrase_table(
            program_options_parser.phrase_table_path(), 5);
        Decoder decoder(&language_model, &alignment_model, &phrase_table, 1000, 500);

        ifstream input_file(program_options_parser.input_file_path());
        if (!input_file) {
            throw runtime_error("Failed to open input file");
        }
        ofstream output_file(program_options_parser.output_file_path());
        if (!output_file) {
            throw runtime_error("Failed to open output file");
        }
        string sentence;
        while (getline(input_file, sentence)) {
            cout << "Translating " << sentence << endl;
            Phrase french_phrase = french_converter.ToIndex(sentence);
            Phrase english_phrase = decoder.decode(french_phrase);
            output_file << english_converter.ToSentence(english_phrase) << endl;
        }
        input_file.close();
        output_file.close();
        return 0;
    } catch (const exception& error) {
        cerr << "Fatal error occurred: " << error.what() << endl;
        return 1;
    }
}
