#ifndef QNVSM_MEMORYUTILIZATIONCONTAINER_H
#define QNVSM_MEMORYUTILIZATIONCONTAINER_H

#include "utilization/UtilizationContainer.h"

class MemoryUtilizationContainer: public UtilizationContainer {
public:
    MemoryUtilizationContainer();

    void onDataUpdated();

protected:
    bool showToolTip(const QPoint &pos, const QString &name, int gpuId) override;
};

#endif //QNVSM_MEMORYUTILIZATIONCONTAINER_H
