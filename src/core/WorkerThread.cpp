#include "WorkerThread.h"

#include <iostream>

#include "Settings.h"

using namespace std;

WorkerThread::WorkerThread() {
    workers = new Worker*[NVSMMaxWorkers];
}

WorkerThread::~WorkerThread() {
    delete[] workers;

    cout << "WorkerThread " << this << " deleted\n";
}

void WorkerThread::run() {
    while (running) {
        for (uint i = 0; i < NVSMMaxWorkers; i++)
            workers[i]->work();

        usleep(UPDATE_DELAY_USEC);
    }

    cout << "WorkerThread done all work!\n";
}
