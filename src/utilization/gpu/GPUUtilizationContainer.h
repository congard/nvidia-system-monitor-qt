#ifndef QNVSM_GPUUTILIZATIONCONTAINER_H
#define QNVSM_GPUUTILIZATIONCONTAINER_H

#include "utilization/UtilizationContainer.h"

class GPUUtilizationContainer final : public UtilizationContainer {
    Q_OBJECT

public:
    GPUUtilizationContainer();
};


#endif //QNVSM_GPUUTILIZATIONCONTAINER_H
