#ifndef QNVSM_GPUUTILIZATIONWORKER_H
#define QNVSM_GPUUTILIZATIONWORKER_H

#include "utilization/UtilizationWorker.h"

class GPUUtilizationWorker: public UtilizationWorker {
public:
    void receiveData() override;
};

#endif //QNVSM_GPUUTILIZATIONWORKER_H
