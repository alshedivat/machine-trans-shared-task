#ifndef ALIGNMENTMODEL_ALIGNMENTMODEL_H
#define ALIGNMENTMODEL_ALIGNMENTMODEL_H

#include <cmath>

class AlignmentModel {
 public:
  AlignmentModel(double alpha=0.5) : alpha_(alpha) {
  }

  double get_probability(int distance) const {
    return log(pow(alpha_, std::abs(distance)));
  }

 private:
  double alpha_;
};

#endif // ALIGNMENTMODEL_ALIGNMENTMODEL_H
