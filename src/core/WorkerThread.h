#ifndef QNVSM_WORKERTHREAD_H
#define QNVSM_WORKERTHREAD_H

#include <QThread>

#include "Worker.h"

class WorkerThread: public QThread {
public:
    WorkerThread();
    ~WorkerThread() override;

    void run() override;

public:
    bool running = true;
    Worker **workers;
};

#endif //QNVSM_WORKERTHREAD_H
