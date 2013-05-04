#ifndef ALIGNMENTMODEL_ALIGNMENTMODEL_H
#define ALIGNMENTMODEL_ALIGNMENTMODEL_H

#include "../phrasetable/phrasetable.h"

class AlignmentModel {
 public:
  double get_probabilty(int distance);
};

#endif // ALIGNMENTMODEL_ALIGNMENTMODEL_H
