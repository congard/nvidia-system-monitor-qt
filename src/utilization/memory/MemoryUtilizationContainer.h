#ifndef QNVSM_MEMORYUTILIZATIONCONTAINER_H
#define QNVSM_MEMORYUTILIZATIONCONTAINER_H

#include "utilization/UtilizationContainer.h"

class MemoryUtilizationContainer: public UtilizationContainer {
public:
    MemoryUtilizationContainer();

    void onDataUpdated();
};

#endif //QNVSM_MEMORYUTILIZATIONCONTAINER_H
