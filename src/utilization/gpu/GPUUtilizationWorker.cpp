#include "GPUUtilizationWorker.h"

#include "constants.h"
#include "core/Utils.h"

using namespace Utils;

void GPUUtilizationWorker::receiveData() {
    std::vector<std::string> lines = split(exec(NVSMI_CMD_GPU_UTILIZATION), "\n");
    // fake, "emulated" GPUs
//    lines[2] = std::to_string(50 + rand() % 20) + " %";
//    lines.push_back(std::to_string(30 + rand() % 10) + " %");
//    lines.push_back(std::to_string(70 + rand() % 30) + " %");
//    lines.push_back("");
    for (size_t i = 1; i < lines.size() - 1; i++)
        udata[i - 1].level = std::atoi(split(lines[i], " ")[0].c_str());
}
