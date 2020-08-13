#ifndef QNVSM_PROCESSESWORKER_H
#define QNVSM_PROCESSESWORKER_H

#include "core/Worker.h"
#include "ProcessInfo.h"

class ProcessesWorker: public Worker {
public:
    void work() override;
    int processesIndexByPid(const QString &pid);

public:
    QVector<ProcessInfo> processes;
};

#endif //QNVSM_PROCESSESWORKER_H
