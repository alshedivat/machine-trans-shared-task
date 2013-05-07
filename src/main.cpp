#include "machinetranslator/machinetranslator.h"
#include "programoptionsparser/programoptionsparser.h"
#include <iostream>
#include <stdexcept>

using std::cerr;
using std::endl;
using std::exception;

int main(int argc, char** argv) {
    try {
        ProgramOptionsParser program_options_parser;
        program_options_parser.Parse(argc, argv);
        MachineTranslator machine_translator;
        machine_translator.Init(
            program_options_parser.english_sentences_path(),
            program_options_parser.vocabulary_path(),
            program_options_parser.language_model_path(),
            program_options_parser.phrase_table_path());
        machine_translator.Translate(
            program_options_parser.input_file_path(),
            program_options_parser.output_file_path());
        return 0;
    } catch (const exception& error) {
        cerr << "Fatal error occurred: " << error.what() << endl;
        return 1;
    }
}
