#include "UtilizationWorker.h"

#include "core/SettingsManager.h"
#include "core/InfoProvider.h"
#include "core/Utils.h"

using namespace Utils;

UtilizationWorker::UtilizationWorker() {
    graphPoints.resize(InfoProvider::getGPUCount());
    utilizationData.resize(InfoProvider::getGPUCount());
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
        auto &points = graphPoints[g];
        auto &data = utilizationData[g];

        for (Point &point : points)
            point.x -= step;

        points.emplace_back(1.0f, data.level);

        // deleteSuperfluousPoints(g)
        if (points.size() > 2 && points[0].x < 0 && points[1].x <= 0) {
            points.erase(points.begin());
        }

        // calculate average, min, max
        data.avgLevel = data.maxLevel = 0;
        data.minLevel = 100;

        for (auto &point : points) {
            data.avgLevel += point.y;

            if (data.maxLevel < point.y) {
                data.maxLevel = point.y;
            }

            if (data.minLevel > point.y) {
                data.minLevel = point.y;
            }
        }

        data.avgLevel /= static_cast<int>(points.size());
    }

    mutex.unlock();
    dataUpdated();

    lastTime = getTime();
}
