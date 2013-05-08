#include "machinetranslator.h"
#include "../alignmentmodel/alignmentmodel.h"
#include "../converter/converter.h"
#include "../decoder/decoder.h"
#include "../languagemodel/languagemodel.h"
#include "../phrasetable/phrasetable.h"
#include <time.h>
#include <boost/thread.hpp>
#include <boost/threadpool.hpp>
#include <fstream>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>

using boost::threadpool::pool;
using std::cout;
using std::getline;
using std::endl;
using std::ifstream;
using std::ofstream;
using std::runtime_error;
using std::shared_ptr;
using std::string;

void MachineTranslator::Init(const string& english_sentences_path,
                             const string& vocabulary_path,
                             const string& language_model_path,
                             const string& phrase_table_path,
                             size_t best_translations_number,
                             double reordering_alpha,
                             double decoder_difference,
                             int decoder_quantity,
                             size_t n_gram_parameter) {
  cout << "Initializing machine translator" << endl;

  shared_ptr<PhraseTable> phrase_table(new PhraseTable());
  PhraseTableLoader phrase_table_loader;
  *phrase_table = phrase_table_loader.load_phrase_table(
      phrase_table_path,
      best_translations_number);
  converter_.reset(new Converter(vocabulary_path));
  shared_ptr<AlignmentModel> alignment_model(
      new AlignmentModel(reordering_alpha));
  shared_ptr<LanguageModel> language_model(new LanguageModel());
  *language_model = LoadLanguageModel(language_model_path,
                                      english_sentences_path,
                                      n_gram_parameter);

  decoder_.reset(new Decoder(language_model,
                             alignment_model,
                             phrase_table,
                             decoder_difference,
                             decoder_quantity));
  cout << "Machine translator initialized" << endl;
}

void MachineTranslator::Translate(const string& input_filename,
                                  const string& output_filename,
                                  size_t max_phrase_length,
                                  size_t max_swap_distance) {
  cout << "Starting translating" << endl;
  ofstream file(output_filename.data());
  if (!file) {
    throw runtime_error("Failed to open file for writing results");
  }
  time_t start, curr;
  time(&start);
  TranslateInThreadPool(input_filename, max_phrase_length, max_swap_distance);
  for (size_t index = 0; index < translations_.size(); ++index) {
    file << translations_[index] << endl;
  }
  time(&curr);
  cout << "Finished translating in " << difftime(curr, start) << endl;
  file.close();
}

LanguageModel MachineTranslator::LoadLanguageModel(
    const string& language_model_path,
    const string& english_sentences_path,
    size_t n_gram_parameter) {
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
    Phrase sent = converter_->ToIndex(sentence);
    sentences.push_back(sent);
  }
  file.close();
  // 91912 - quantity of ENGLISH words;
  LanguageModel language_model =
      learn_ngram_language_model(sentences,
                                 kEnglishDictSize_,
                                 n_gram_parameter);
  language_model.save(language_model_path);
  return language_model;
}

void MachineTranslator::TranslateInThreadPool(const string& filename,
                                              size_t max_phrase_length,
                                              size_t max_swap_distance) {
  ifstream file(filename.data());
  if (!file) {
    throw runtime_error("Failed to open file with input data");
  }
  string sentence;
  size_t number_sentences = 0;
  while (getline(file, sentence)) {
    ++number_sentences;
  }
  file.close();
  translations_.resize(number_sentences);
  file.open(filename.data());
  if (!file) {
    throw runtime_error("Failed to open fil with input data");
  }
  pool thread_pool(boost::thread::hardware_concurrency());
  number_sentences = 0;
  while (getline(file, sentence)) {
    thread_pool.schedule(boost::bind(&MachineTranslator::TranslateSentence,
                                     this,
                                     sentence,
                                     number_sentences,
                                     max_phrase_length,
                                     max_swap_distance));
    ++number_sentences;
  }
}
