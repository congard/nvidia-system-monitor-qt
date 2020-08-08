#ifndef QNVSM_PROCESSESWORKER_H
#define QNVSM_PROCESSESWORKER_H

#include "core/Worker.h"
#include "ProcessInfo.h"

class ProcessesWorker: public Worker {
public:
    void work() override;
    int processesIndexByPid(const std::string &pid);

public:
    std::vector<ProcessInfo> processes;
};

#endif //QNVSM_PROCESSESWORKER_H
