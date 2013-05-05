#include "programoptionsparser.h"
#include <boost/program_options.hpp>
#include <fstream>
#include <string>

using namespace boost::program_options;
using std::ifstream;
using std::string;

void ProgramOptionsParser::Parse(int argc, char** argv) {
  variables_map program_options_storage;
  string configuration_filename = "machine_translator.conf";
  options_description program_options_description("Options for program");
  program_options_description.add_options()
      ("configuration_filename,c", value<string>(&configuration_filename),
        "Name of configuration file")
      ("english_sentences_path,s", value<string>(&english_sentences_path_),
        "Name of file containing english corpus")
      ("english_vocabulary_path,e", value<string>(&english_vocabulary_path_),
        "Name of file containing english vocabulary")
      ("french_vocabulary_path,f", value<string>(&french_vocabulary_path_),
        "Name of file containing french vocabulary")
      ("language_model_path,l", value<string>(&language_model_path_),
        "Name of file containing language model")
      ("phrase_table_path,p", value<string>(&phrase_table_path_),
        "name of file containing phrase table");
  store(parse_command_line(argc, argv, program_options_description),
        program_options_storage);
  notify(program_options_storage);
  ifstream file(configuration_filename.data());
  if (file) {
    file.close();
    store(parse_config_file<char>(configuration_filename.data(),
                                  program_options_description),
          program_options_storage);
    notify(program_options_storage);
  }
}