#ifndef QNVSM_MEMORYUTILIZATIONWIDGET_H
#define QNVSM_MEMORYUTILIZATIONWIDGET_H

#include "utilization/UtilizationWidget.h"

class MemoryUtilizationWidget: public UtilizationWidget {
public:
    MemoryUtilizationWidget();

    void mouseMoveEvent(QMouseEvent *event) override;
};

#endif //QNVSM_MEMORYUTILIZATIONWIDGET_H
