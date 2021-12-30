#ifndef QNVSM_POWERWORKER_H
#define QNVSM_POWERWORKER_H

#include "core/Worker.h"

class PowerWorker: public Worker {
public:
    void work() override;
};

#endif //QNVSM_POWERWORKER_H
