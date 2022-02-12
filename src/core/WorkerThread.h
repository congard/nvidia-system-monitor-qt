#ifndef QNVSM_WORKERTHREAD_H
#define QNVSM_WORKERTHREAD_H

#include <QThread>
#include <QVarLengthArray>

#include "Worker.h"

class WorkerThread: public QThread {
public:
    constexpr static int WorkersCount = 1;

public:
    WorkerThread();
    ~WorkerThread() override;

    void run() override;

public:
    bool running = true;
    QVarLengthArray<Worker*> workers;
};

#endif //QNVSM_WORKERTHREAD_H
