#include "GPUUtilizationWorker.h"

#include "core/InfoProvider.h"

void GPUUtilizationWorker::receiveData() {
    for (int i = 0; i < InfoProvider::getGPUCount(); i++) {
        udata[i].level = InfoProvider::getGPUUtil(i);
    }
}
