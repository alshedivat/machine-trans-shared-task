#ifndef ALIGNMENTMODEL_ALIGNMENTMODEL_H
#define ALIGNMENTMODEL_ALIGNMENTMODEL_H

#include <cmath>

class AlignmentModel {
 public:
  AlignmentModel(double alpha=0.5) : alpha_(alpha) {
  }

  double get_probabilty(int distance) {
    return pow(alpha_, fabs(distance));
  }

 private:
  double alpha_;
};

#endif // ALIGNMENTMODEL_ALIGNMENTMODEL_H
