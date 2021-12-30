#ifndef QNVSM_UTILIZATIONWORKER_H
#define QNVSM_UTILIZATIONWORKER_H

#include "core/Worker.h"
#include "Point.h"
#include "UtilizationData.h"

class UtilizationWorker: public Worker {
public:
    UtilizationWorker();
    ~UtilizationWorker() override;

    void work() override;
    void deleteSuperfluousPoints(int index);
    virtual void receiveData() = 0;

public:
    std::vector<Point> *gpoints; // graph points
    UtilizationData *udata;

private:
    long lastTime = 0;
};

#endif //QNVSM_UTILIZATIONWORKER_H
