#include <fstream>
#include <iostream>
#include <stdexcept>
#include "alignmentmodel/alignmentmodel.h"
#include "converter/converter.h"
#include "decoder/decoder.h"
#include "languagemodel/languagemodel.h"
#include "phrasetable/phrasetable.h"

using std::cerr;
using std::cin;
using std::cout;
using std::getline;
using std::endl;
using std::exception;
using std::ifstream;
using std::runtime_error;
using std::string;

LanguageModel loadLanguageModel(const Converter& converter) {
    ifstream file(LANGUAGE_MODEL_PATH);
    if (file) {
        file.close();
        return load_ngram_language_model(LANGUAGE_MODEL_PATH);
    }
    file.open(ENGLISH_SENTENCES_PATH);
    if (!file) {
        throw runtime_error("Failed to load english sentences");
    }
    string sentence;
    vector<Phrase> sentences;
    while (getline(file, sentence)) {
        sentences.push_back(converter.ToIndex(sentence));
    }
    file.close();
    cout << "Load all files" << endl;
    return learn_ngram_language_model(sentences, converter.DictSize());
}

int main() {
    try {
        Converter english_converter(ENGLISH_VOCABULARY_PATH);
        Converter french_converter(FRENCH_VOCABULARY_PATH);
        LanguageModel language_model = loadLanguageModel(english_converter);
        cout << "Load language model" << endl;
        AlignmentModel alignment_model;
        cout << "Alignment model created" << endl;
        PhraseTable phrase_table = load_phrase_table(PHRASE_TABLE_PATH, 5);
        Decoder decoder(&language_model, &alignment_model, &phrase_table, 100, 100);
        cout << "Started translating" << endl;
        string sentence;
        while (getline(cin, sentence)) {
            Phrase french_phrase = french_converter.ToIndex(sentence);
            cout << "Readed: " << sentence << " ||| Converted to: ";
            for (size_t i = 0; i < french_phrase.size(); ++i) {
                cout << french_phrase[i] << " ";
            }
            cout << endl;
            Phrase english_phrase = decoder.decode(french_phrase);
            cout << "Decoded to: ";
            for (size_t i = 0; i < french_phrase.size(); ++i) {
                cout << english_phrase[i] << " ";
            }
            cout << "||| Converted to: ";
            cout << english_converter.ToSentence(english_phrase) << endl;
        }
        return 0;
    } catch (const exception& error) {
        cerr << "Fatal error occurred: " << error.what() << endl;
        return 1;
    }
}
