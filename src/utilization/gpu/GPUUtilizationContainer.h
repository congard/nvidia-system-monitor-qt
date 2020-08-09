#ifndef QNVSM_GPUUTILIZATIONCONTAINER_H
#define QNVSM_GPUUTILIZATIONCONTAINER_H

#include "utilization/UtilizationContainer.h"
#include "GPUUtilizationWidget.h"

class GPUUtilizationContainer: public UtilizationContainer {
public:
    explicit GPUUtilizationContainer(GPUUtilizationWidget *utilizationWidget);
};


#endif //QNVSM_GPUUTILIZATIONCONTAINER_H
