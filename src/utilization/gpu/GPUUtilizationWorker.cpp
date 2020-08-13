#include "GPUUtilizationWorker.h"

#include "core/NVSMIParser.h"

void GPUUtilizationWorker::receiveData() {
    auto gpuUtilization = NVSMIParser::getGPUUtilization();

    for (size_t i = 0; i < gpuUtilization.size(); i++)
        udata[i].level = gpuUtilization[i];
}
