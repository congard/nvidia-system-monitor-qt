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
                data[NVSMIProcesses::Name], data[NVSMIProcesses::Type],
                data[NVSMIProcesses::GPUIdx], data[NVSMIProcesses::PID],
                data[NVSMIProcesses::Sm], data[NVSMIProcesses::Mem],
                data[NVSMIProcesses::Enc], data[NVSMIProcesses::Dec],
                data[NVSMIProcesses::FbMem]
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
