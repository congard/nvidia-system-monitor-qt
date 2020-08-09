#ifndef QNVSM_MEMORYUTILIZATIONCONTAINER_H
#define QNVSM_MEMORYUTILIZATIONCONTAINER_H

#include "utilization/UtilizationContainer.h"
#include "MemoryUtilizationWidget.h"

class MemoryUtilizationContainer: public UtilizationContainer {
public:
    explicit MemoryUtilizationContainer(MemoryUtilizationWidget *utilizationWidget);
};

#endif //QNVSM_MEMORYUTILIZATIONCONTAINER_H
