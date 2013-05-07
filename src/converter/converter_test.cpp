#include <iostream>
#include "converter.h"
#include "converter.cpp"

int main() {
  Converter en_converter("../../data/corpus/en.vcb");

  // Test correctness on existing words
  vector<size_t> indices = en_converter.ToIndex("resumption of the session");
  for (size_t index : indices) {
    std::cout << index << " ";
  }
  std::cout << std::endl;

  // Test back conversion
  string sentence = en_converter.ToSentence(indices);
  std::cout << sentence << std::endl;

  // Test dictionary size output
  std::cout << en_converter.DictSize() << std::endl;

  // Test on non-existing words
  indices = en_converter.ToIndex("fsjdhf jsdhfjksh sdfhsdkjfh jdfhjdhf sfhdj");
  for (size_t index : indices) {
    std::cout << index << " ";
  }
  std::cout << std::endl;

  sentence = en_converter.ToSentence(indices);
  std::cout << sentence << std::endl;

  // Test dictionary size output
  std::cout << en_converter.DictSize() << std::endl;

  return 0;
}
