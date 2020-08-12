#include "ProcessesWorker.h"

#include "core/NVSMIParser.h"

void ProcessesWorker::work() {
    {
        QMutexLocker locker(&mutex);
        processes = NVSMIParser::getProcesses();
    }

    dataUpdated();
}

int ProcessesWorker::processesIndexByPid(const QString &pid) {
    for (size_t i = 0; i < processes.size(); i++)
        if (processes[i].pid == pid)
            return i;

    return -1;
}
