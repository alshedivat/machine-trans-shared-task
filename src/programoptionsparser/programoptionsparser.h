#include <string>

class ProgramOptionsParser {
 public:
  ProgramOptionsParser()
   : english_sentences_path_(""),
     vocabulary_path_(""),
     input_file_path_(""),
     language_model_path_(""),
     output_file_path_(""),
     phrase_table_path_("") {
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

 private:
  std::string english_sentences_path_;
  std::string vocabulary_path_;
  std::string input_file_path_;
  std::string language_model_path_;
  std::string output_file_path_;
  std::string phrase_table_path_;
};

