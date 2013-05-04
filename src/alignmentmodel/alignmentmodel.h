#ifndef ALIGNMENTMODEL_ALIGNMENTMODEL_H
#define ALIGNMENTMODEL_ALIGNMENTMODEL_H

#include <math.h>

#include "../phrasetable/phrasetable.h"

class AlignmentModel {
 public:
  AlignmentModel(const double alpha)
	  : alpha_(alpha)
  {
  }

  double get_probabilty(int distance) const {
	  return pow(alpha_, abs(distance));
  }

private:

  double alpha_;
};

#endif // ALIGNMENTMODEL_ALIGNMENTMODEL_H
