#ifndef QNVSM_MEMORYUTILIZATIONCONTAINER_H
#define QNVSM_MEMORYUTILIZATIONCONTAINER_H

#include "utilization/UtilizationContainer.h"

class MemoryUtilizationContainer: public UtilizationContainer {
    Q_OBJECT

public:
    MemoryUtilizationContainer();

public slots:
    void onDataUpdated();
};

#endif //QNVSM_MEMORYUTILIZATIONCONTAINER_H
