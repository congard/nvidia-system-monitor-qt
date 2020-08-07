#ifndef QNVSM_WORKER_H
#define QNVSM_WORKER_H

#include <QObject>
#include <QMutex>

class Worker: public QObject {
    Q_OBJECT

public:
    ~Worker() override;

    virtual void work() = 0;

public:
    QMutex mutex;

signals:
    void dataUpdated();
};

#endif
