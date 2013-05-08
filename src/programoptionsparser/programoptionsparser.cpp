#include "programoptionsparser.h"
#include <boost/program_options.hpp>
#include <fstream>
#include <iostream>
#include <string>

using namespace boost::program_options;
using std::cout;
using std::endl;
using std::ifstream;
using std::string;

void ProgramOptionsParser::Parse(int argc, char** argv) {
  cout << "Parsing program options" << endl;
  variables_map program_options_storage;
  string configuration_filename = "machine_translator.conf";
  options_description program_options_description("Options for program");
  program_options_description.add_options()
      ("configuration_filename,c", value<string>(&configuration_filename),
        "Name of configuration file")
      ("english_sentences_path,s", value<string>(&english_sentences_path_),
        "Name of file containing english corpus")
      ("vocabulary_path,f", value<string>(&vocabulary_path_),
        "Name of file containing vocabulary")
      ("input_file_path,i", value<string>(&input_file_path_),
        "Name of input file")
      ("language_model_path,l", value<string>(&language_model_path_),
        "Name of file containing language model")
      ("output_file_path,o", value<string>(&output_file_path_),
        "Name of output file")
      ("phrase_table_path,p", value<string>(&phrase_table_path_),
        "Name of file containing phrase table")
      ("best_translations_number,b", value<size_t>(&best_translations_number_),
        "Number of best translations to remain")
      ("reordering_alpha,r", value<double>(&reordering_alpha_),
        "Reordering parameter")
      ("decoder_difference,d", value<double>(&decoder_difference_),
        "Decoder difference")
      ("decoder_quantity,q", value<int>(&decoder_quantity_),
        "Decoder quantity")
      ("n_gram_parameter,n", value<size_t>(&n_gram_parameter_),
        "N gram parameter")
      ("max_phrase_length,m", value<size_t>(&max_phrase_length_),
        "Maximum phrase length")
      ("max_swap_distance,w", value<size_t>(&max_swap_distance_),
        "Maximum swap distance");
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

