#ifndef QNVSM_MEMORYUTILIZATIONWORKER_H
#define QNVSM_MEMORYUTILIZATIONWORKER_H

#include "utilization/UtilizationWorker.h"
#include "MemoryData.h"

class MemoryUtilizationWorker: public UtilizationWorker {
public:
    MemoryUtilizationWorker();
    ~MemoryUtilizationWorker() override;

    void receiveData() override;

public:
    MemoryData *memoryData;
};

#endif //QNVSM_MEMORYUTILIZATIONWORKER_H
