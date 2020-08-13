#include "MemoryUtilizationWorker.h"

#include "core/NVSMIParser.h"

void MemoryUtilizationWorker::receiveData() {
    memoryData = NVSMIParser::getMemoryUtilization();

    for (int i = 0; i < memoryData.size(); i++)
        udata[i].level = memoryData[i].used * 100 / memoryData[i].total;
}
