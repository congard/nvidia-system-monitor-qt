#include "UtilizationWorker.h"

#include "core/SettingsManager.h"
#include "core/InfoProvider.h"
#include "core/Utils.h"

using namespace Utils;

UtilizationWorker::UtilizationWorker() {
    gpoints = new std::vector<Point>[InfoProvider::getGPUCount()];
    udata = new UtilizationData[InfoProvider::getGPUCount()];
}

UtilizationWorker::~UtilizationWorker() {
    delete[] gpoints;
    delete[] udata;
}

void UtilizationWorker::work() {
    // TODO: move this code directly to UtilizationWidget?

    mutex.lock();

    if (lastTime == 0) {
        lastTime = getTime();
    }

    receiveData();

    auto graphStep = (float) SettingsManager::getUpdateDelay() / (float) SettingsManager::getGraphLength();
    auto step = (float)(getTime() - lastTime) / (float) SettingsManager::getUpdateDelay() * graphStep;

    // g means gpu
    for (int g = 0; g < InfoProvider::getGPUCount(); g++) {
        for (Point &i : gpoints[g])
            i.x -= step;

        gpoints[g].emplace_back(1.0f, udata[g].level);

        deleteSuperfluousPoints(g);

        // calculate average, min, max
        udata[g].avgLevel = udata[g].maxLevel = 0;
        udata[g].minLevel = 100;

        for (size_t i = 0; i < gpoints[g].size(); i++) {
            udata[g].avgLevel += gpoints[g][i].y;

            if (udata[g].maxLevel < gpoints[g][i].y) {
                udata[g].maxLevel = gpoints[g][i].y;
            }

            if (udata[g].minLevel > gpoints[g][i].y) {
                udata[g].minLevel = gpoints[g][i].y;
            }
        }

        udata[g].avgLevel /= static_cast<int>(gpoints[g].size());
    }

    mutex.unlock();
    dataUpdated();

    lastTime = getTime();
}

void UtilizationWorker::deleteSuperfluousPoints(int index) {
    if (gpoints[index].size() > 2 && gpoints[index][0].x < 0 && gpoints[index][1].x <= 0) {
        gpoints[index].erase(gpoints[index].begin());
    }
}
