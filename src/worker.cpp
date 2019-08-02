#include "worker.h"

#include <iostream>

#include "utils.h"
#include "constants.h"
#include "settings.h"

void Worker::work() {}

Worker::~Worker() {
    std::cout << "Worker " << this << " deleted\n";
}

WorkerThread::WorkerThread() {
    workers = new Worker*[NVSM_WORKERS_MAX];
}

void WorkerThread::run() {
    while (running) {
        for (uint i = 0; i < NVSM_WORKERS_MAX; i++) {
            workers[i]->work();
        }

        usleep(UPDATE_DELAY_USEC);
    }

    std::cout << "WorkerThread done all work!\n";
}

WorkerThread::~WorkerThread() {
    delete[] workers;
    std::cout << "WorkerThread " << this << " deleted\n";
}
