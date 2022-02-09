#include "MemoryUtilizationWorker.h"

#include "core/InfoProvider.h"

void MemoryUtilizationWorker::receiveData() {
    for (int i = 0; i < InfoProvider::getGPUCount(); i++) {
        utilizationData[i].level = InfoProvider::getMemUsed(i) * 100 / InfoProvider::getMemTotal(i);
    }
}
