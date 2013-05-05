#include <string>

class ProgramOptionsParser {
 public:
  ProgramOptionsParser()
   : english_sentences_path_(""),
     english_vocabulary_path_(""),
     french_vocabulary_path_(""),
     language_model_path_(""),
     phrase_table_path_("") {
  }

  void Parse(int argc, char** argv);

  std::string english_sentences_path() {
    return english_sentences_path_;
  }

  std::string english_vocabulary_path() {
    return english_vocabulary_path_;
  }

  std::string french_vocabulary_path() {
    return french_vocabulary_path_;
  }

  std::string language_model_path() {
    return language_model_path_;
  }

  std::string phrase_table_path() {
    return phrase_table_path_;
  }

 private:
  std::string english_sentences_path_;
  std::string english_vocabulary_path_;
  std::string french_vocabulary_path_;
  std::string language_model_path_;
  std::string phrase_table_path_;
};