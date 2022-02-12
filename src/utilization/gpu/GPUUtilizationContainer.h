#ifndef QNVSM_GPUUTILIZATIONCONTAINER_H
#define QNVSM_GPUUTILIZATIONCONTAINER_H

#include "utilization/UtilizationContainer.h"

class GPUUtilizationContainer: public UtilizationContainer {
public:
    GPUUtilizationContainer();

    void onDataUpdated();
};


#endif //QNVSM_GPUUTILIZATIONCONTAINER_H
