#include "GPUUtilizationWorker.h"

#include "core/InfoProvider.h"

void GPUUtilizationWorker::receiveData() {
    for (int i = 0; i < InfoProvider::getGPUCount(); i++) {
        utilizationData[i].level = InfoProvider::getGPUUtil(i);
    }
}
