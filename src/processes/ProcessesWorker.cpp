#include "ProcessesWorker.h"

#include "constants.h"
#include "core/Utils.h"

using namespace Utils;

void ProcessesWorker::work() {
    mutex.lock();

    std::vector<std::string> lines = split(streamline(exec(NVSMI_CMD_PROCESSES)), "\n"), data;
    processes.clear();

    for (size_t i = 2; i < lines.size(); i++) {
        if (lines[i].empty())
            continue;

        data = split(lines[i], " ");

        processes.emplace_back(
                data[NVSMI_NAME], data[NVSMI_TYPE],
                data[NVSMI_GPUIDX], data[NVSMI_PID],
                data[NVSMI_SM], data[NVSMI_MEM],
                data[NVSMI_ENC], data[NVSMI_DEC],
                data[NVSMI_FBMEM]
        );
    }

    mutex.unlock();

    dataUpdated();
}

int ProcessesWorker::processesIndexByPid(const std::string& pid) {
    for (size_t i = 0; i < processes.size(); i++)
        if (processes[i].pid == pid)
            return i;

    return -1;
}
