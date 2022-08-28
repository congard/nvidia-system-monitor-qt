#ifndef QNVSM_PCHIPINTERPOLATOR_H
#define QNVSM_PCHIPINTERPOLATOR_H

#include <vector>
#include "Point.h"

// Based on algine::PCHIPInterpolator
// https://github.com/congard/algine

class PCHIPInterpolator {
public:
    static std::vector<PointF> getInterpolatedPoints(const std::vector<PointF> &src, PointF scale, int count);
};

#endif //QNVSM_PCHIPINTERPOLATOR_H
