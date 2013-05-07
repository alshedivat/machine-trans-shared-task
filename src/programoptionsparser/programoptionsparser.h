#include <string>

class ProgramOptionsParser {
 public:
  ProgramOptionsParser()
   : english_sentences_path_(""),
     vocabulary_path_(""),
     input_file_path_(""),
     language_model_path_(""),
     output_file_path_(""),
     phrase_table_path_(""),
     best_translations_number_(5),
     reordering_alpha_(0.5),
     decoder_difference_(100),
     decoder_quantity_(300),
     n_gram_parameter_(3),
     max_phrase_length_(6),
     max_swap_distance_(6) {
  }

  void Parse(int argc, char** argv);

  std::string english_sentences_path() {
    return english_sentences_path_;
  }

  std::string vocabulary_path() {
    return vocabulary_path_;
  }

  std::string input_file_path() {
    return input_file_path_;
  }

  std::string language_model_path() {
    return language_model_path_;
  }

  std::string output_file_path() {
    return output_file_path_;
  }

  std::string phrase_table_path() {
    return phrase_table_path_;
  }

  size_t best_translations_number() {
    return best_translations_number_;
  }

  double reordering_alpha() {
    return reordering_alpha_;
  }

  int decoder_difference() {
    return decoder_difference_;
  }

  int decoder_quantity() {
    return decoder_quantity_;
  }

  size_t n_gram_parameter() {
    return n_gram_parameter_;
  }

  size_t max_phrase_length() {
    return max_phrase_length_;
  }

  size_t max_swap_distance() {
    return max_swap_distance_;
  }

 private:
  std::string english_sentences_path_;
  std::string vocabulary_path_;
  std::string input_file_path_;
  std::string language_model_path_;
  std::string output_file_path_;
  std::string phrase_table_path_;
  size_t best_translations_number_;
  double reordering_alpha_;
  int decoder_difference_;
  int decoder_quantity_;
  size_t n_gram_parameter_;
  size_t max_phrase_length_;
  size_t max_swap_distance_;
};

