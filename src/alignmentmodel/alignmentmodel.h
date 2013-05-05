#ifndef ALIGNMENTMODEL_ALIGNMENTMODEL_H
#define ALIGNMENTMODEL_ALIGNMENTMODEL_H

#include <cmath>

class AlignmentModel {
 public:
  AlignmentModel(double alpha=0.5) : alpha_(alpha) {
  }

  double get_probability(int distance) const {
    return log(pow(alpha_, std::fabs(static_cast<double>(distance))));
  }

 private:
  double alpha_;
};

#endif // ALIGNMENTMODEL_ALIGNMENTMODEL_H
