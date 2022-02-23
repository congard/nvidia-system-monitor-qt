#ifndef QNVSM_GPUUTILIZATIONCONTAINER_H
#define QNVSM_GPUUTILIZATIONCONTAINER_H

#include "utilization/UtilizationContainer.h"

class GPUUtilizationContainer: public UtilizationContainer {
public:
    GPUUtilizationContainer();

    void onDataUpdated();

protected:
    bool showToolTip(const QPoint& pos, const QString &name, int gpuId) override;
};


#endif //QNVSM_GPUUTILIZATIONCONTAINER_H
