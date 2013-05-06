#include "../converter/converter.h"
#include "../decoder/decoder.h"
#include "../languagemodel/languagemodel.h"
#include "../phrasetable/phrasetable.h"
#include <memory>
#include <string>
#include <vector>

class MachineTranslator {
 public:
  void Init(const std::string& english_sentences_path,
            const std::string& english_vocabulary_path,
            const std::string& french_vocabulary_path,
            const std::string& language_model_path,
            const std::string& phrase_table_path);

  void Translate(const std::string& input_filename,
                 const std::string& output_filename);
 private:
  LanguageModel LoadLanguageModel(const std::string& english_sentences_path,
                                  const std::string& language_model_path);
  void TranslateInThreadPool(const std::string& filename);

  void TranslateSentence(const std::string& sentence,
                         size_t sentence_number) {
    Phrase french_phrase = french_converter_->ToIndex(sentence);
    Phrase english_phrase = decoder_->decode(french_phrase, 6, 6);
    translations_[sentence_number] = english_converter_->ToSentence(
        english_phrase);
  }

  std::shared_ptr<Converter> english_converter_;
  std::shared_ptr<Converter> french_converter_;
  std::shared_ptr<Decoder> decoder_;
  std::vector<std::string> translations_;
};