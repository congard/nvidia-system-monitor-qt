#ifndef WORKER_H
#define WORKER_H

#include <QThread>
#include <QMutex>

class Worker : public QObject {
    Q_OBJECT
public:
    QMutex mutex;

    virtual void work() = 0;

    ~Worker() override;
signals:
    void dataUpdated();
};

class WorkerThread : public QThread {
public:
    bool running = true;
    Worker **workers;
    
    WorkerThread();
    ~WorkerThread() override;
    
    void run() override;
};

#endif
