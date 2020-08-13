#ifndef QNVSM_MEMORYUTILIZATIONWORKER_H
#define QNVSM_MEMORYUTILIZATIONWORKER_H

#include "utilization/UtilizationWorker.h"
#include "MemoryData.h"

class MemoryUtilizationWorker: public UtilizationWorker {
public:
    void receiveData() override;

public:
    QVarLengthArray<MemoryData> memoryData;
};

#endif //QNVSM_MEMORYUTILIZATIONWORKER_H
