#ifndef LANGUAGEMODEL_H
#define LANGUAGEMODEL_H

#include "../phrasetable/phrasetable.h"

class LanguageModel {
 public:
  double get_probability(const Phrase & phrase);

};


#endif // LANGUAGEMODEL_H
