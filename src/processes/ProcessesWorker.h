#ifndef QNVSM_PROCESSESWORKER_H
#define QNVSM_PROCESSESWORKER_H

#include "core/Worker.h"
#include "ProcessList.h"

class ProcessesWorker: public Worker {
public:
    void work() override;
    int processesIndexByPid(const std::string &pid);

public:
    std::vector<ProcessList> processes;
};

#endif //QNVSM_PROCESSESWORKER_H
