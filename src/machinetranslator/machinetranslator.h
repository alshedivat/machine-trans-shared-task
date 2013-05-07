#include "../converter/converter.h"
#include "../decoder/decoder.h"
#include "../languagemodel/languagemodel.h"
#include "../phrasetable/phrasetable.h"
#include <memory>
#include <string>
#include <vector>

class MachineTranslator {
 public:
  MachineTranslator() : kEnglishDictSize_(91912) {
  }

  void Init(const std::string& english_sentences_path,
            const std::string& vocabulary_path,
            const std::string& language_model_path,
            const std::string& phrase_table_path,
            size_t best_translations_number,
            double reordering_alpha,
            int decoder_difference,
            int decoder_quantity,
            size_t n_gram_parameter);

  void Translate(const std::string& input_filename,
                 const std::string& output_filename,
                 size_t max_phrase_length,
                 size_t max_swap_distance);
 private:
  LanguageModel LoadLanguageModel(const std::string& english_sentences_path,
                                  const std::string& language_model_path,
                                  size_t n_gram_parameter);

  void TranslateInThreadPool(const std::string& filename,
                             size_t max_phrase_length,
                             size_t max_swap_distance);

  void TranslateSentence(const std::string& sentence,
                         size_t sentence_number,
                         size_t max_phrase_length,
                         size_t max_swap_distance) {
    Phrase french_phrase = converter_->ToIndex(sentence);
    Phrase english_phrase = decoder_->decode(french_phrase,
                                             max_phrase_length,
                                             max_swap_distance);
    translations_[sentence_number] = converter_->ToSentence(
        english_phrase);
  }

  const size_t kEnglishDictSize_;
  std::shared_ptr<Converter> converter_;
  std::shared_ptr<Decoder> decoder_;
  std::vector<std::string> translations_;
};
