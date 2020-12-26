#ifndef QNVSM_GPUUTILIZATIONCONTAINER_H
#define QNVSM_GPUUTILIZATIONCONTAINER_H

#include "utilization/UtilizationContainer.h"

class GPUUtilizationContainer: public UtilizationContainer {
    Q_OBJECT

public:
    GPUUtilizationContainer();

public slots:
    void onDataUpdated();
};


#endif //QNVSM_GPUUTILIZATIONCONTAINER_H
