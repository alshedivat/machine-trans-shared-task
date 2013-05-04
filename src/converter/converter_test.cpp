#include <iostream>
#include "converter.h"

int main() {
  Converter en_converter("../../data/corpus/en.vcb");

  vector<size_t> indices = en_converter.ToIndex("resumption of the session");
  for (size_t index : indices) {
    std::cout << index << " ";
  }
  std::cout << std::endl;

  string sentence = en_converter.ToSentence(indices);
  std::cout << sentence << std::endl;

  std::cout << en_converter.DictSize() << std::endl;

  return 0;
}
