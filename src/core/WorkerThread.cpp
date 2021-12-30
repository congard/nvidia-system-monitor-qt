#include "WorkerThread.h"

#include <iostream>

#include "SettingsManager.h"

using namespace std;

WorkerThread::WorkerThread() {
    workers.resize(WorkersCount);
}

WorkerThread::~WorkerThread() {
    cout << "WorkerThread " << this << " deleted\n";
}

void WorkerThread::run() {
    while (running) {
        for (int i = 0; i < WorkersCount; i++) {
            workers[i]->work();
        }

        usleep(SettingsManager::getUpdateDelay() * 1000);
    }

    cout << "WorkerThread done all work!\n";
}
