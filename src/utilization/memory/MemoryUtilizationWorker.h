#ifndef QNVSM_MEMORYUTILIZATIONWORKER_H
#define QNVSM_MEMORYUTILIZATIONWORKER_H

#include "utilization/UtilizationWorker.h"

class MemoryUtilizationWorker: public UtilizationWorker {
public:
    void receiveData() override;
};

#endif //QNVSM_MEMORYUTILIZATIONWORKER_H
