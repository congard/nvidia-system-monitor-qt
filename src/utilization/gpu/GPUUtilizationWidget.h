#ifndef QNVSM_GPUUTILIZATIONWIDGET_H
#define QNVSM_GPUUTILIZATIONWIDGET_H

#include "utilization/UtilizationWidget.h"

class GPUUtilizationWidget: public UtilizationWidget {
public:
    GPUUtilizationWidget();

    void mouseMoveEvent(QMouseEvent *event) override;
};

#endif //QNVSM_GPUUTILIZATIONWIDGET_H
