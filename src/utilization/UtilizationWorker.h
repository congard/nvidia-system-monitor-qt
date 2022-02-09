#ifndef QNVSM_UTILIZATIONWORKER_H
#define QNVSM_UTILIZATIONWORKER_H

#include "core/Worker.h"
#include "Point.h"
#include "UtilizationData.h"

class UtilizationWorker: public Worker {
public:
    UtilizationWorker();

    void work() override;
    virtual void receiveData() = 0;

public:
    QVarLengthArray<std::vector<Point>> graphPoints;
    QVarLengthArray<UtilizationData> utilizationData;

private:
    long lastTime = 0;
};

#endif //QNVSM_UTILIZATIONWORKER_H
