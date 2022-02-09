#include "ProcessesWorker.h"

#include "core/InfoProvider.h"

void ProcessesWorker::work() {
    {
        QMutexLocker locker(&mutex);
        processes = InfoProvider::getProcesses(); // TODO: extra copy
    }

    dataUpdated();
}

int ProcessesWorker::processesIndexByPid(const QString &pid) {
    for (size_t i = 0; i < processes.size(); i++)
        if (processes[i].pid == pid)
            return i;

    return -1;
}
