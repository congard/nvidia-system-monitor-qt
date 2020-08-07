#include "UtilizationWorker.h"

#include "Settings.h"
#include "core/Utils.h"

using namespace Utils;

UtilizationWorker::UtilizationWorker() {
    gpoints = new std::vector<Point>[Settings::GPUCount];
    udata = new UtilizationData[Settings::GPUCount];
}

UtilizationWorker::~UtilizationWorker() {
    delete[] gpoints;
    delete[] udata;
}

void UtilizationWorker::work() {
    mutex.lock();

    if (lastTime == 0) {
        lastTime = getTime();
        mutex.unlock();
        return;
    }

    receiveData();

    float step = (float)(getTime() - lastTime) / Settings::UpdateDelay * GRAPH_STEP;

    // g means gpu
    for (int g = 0; g < Settings::GPUCount; g++) {
        for (Point &i : gpoints[g])
            i.x -= step;

        gpoints[g].emplace_back(1.0f, udata[g].level);
        deleteSuperfluousPoints(g);

        // calculate average, min, max
        udata[g].avgLevel = udata[g].maxLevel = 0;
        udata[g].minLevel = 100;
        for (size_t i = 0; i < gpoints[g].size(); i++) {
            udata[g].avgLevel += gpoints[g][i].y;
            if (udata[g].maxLevel < gpoints[g][i].y)
                udata[g].maxLevel = gpoints[g][i].y;
            if (udata[g].minLevel > gpoints[g][i].y)
                udata[g].minLevel = gpoints[g][i].y;
        }
        udata[g].avgLevel /= gpoints[g].size();
    }

    mutex.unlock();
    dataUpdated();

    lastTime = getTime();
}

void UtilizationWorker::deleteSuperfluousPoints(const uint index) {
    if (gpoints[index].size() > 2 && gpoints[index][0].x < 0 && gpoints[index][1].x <= 0)
        gpoints[index].erase(gpoints[index].begin());
}
