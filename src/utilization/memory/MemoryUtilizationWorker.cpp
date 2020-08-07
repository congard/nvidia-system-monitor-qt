#include "MemoryUtilizationWorker.h"

#include "Settings.h"
#include "constants.h"
#include "core/Utils.h"

using namespace Utils;

MemoryUtilizationWorker::MemoryUtilizationWorker() : UtilizationWorker() {
    memoryData = new MemoryData[Settings::GPUCount];
}

MemoryUtilizationWorker::~MemoryUtilizationWorker() {
    delete[] memoryData;
}

void MemoryUtilizationWorker::receiveData() {
    std::vector<std::string> lines = split(exec(NVSMI_CMD_MEM_UTILIZATION), "\n"), data;
    for (size_t i = 1; i < lines.size() - 1; i++) {
        data = split(lines[i], ", ");
        memoryData[i - 1].total = std::atoi(split(data[1], " ")[0].c_str());
        memoryData[i - 1].free = std::atoi(split(data[2], " ")[0].c_str());
        memoryData[i - 1].used = std::atoi(split(data[3], " ")[0].c_str());
        udata[i - 1].level = memoryData[i - 1].used * 100 / memoryData[i - 1].total;
    }
}
