#ifndef QNVSM_GPUUTILIZATIONWIDGET_H
#define QNVSM_GPUUTILIZATIONWIDGET_H

#include "utilization/UtilizationWidget.h"

class GPUUtilizationWidget: public UtilizationWidget {
public:
    void onDataUpdated() override;
};

#endif //QNVSM_GPUUTILIZATIONWIDGET_H
