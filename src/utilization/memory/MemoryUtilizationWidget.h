#ifndef QNVSM_MEMORYUTILIZATIONWIDGET_H
#define QNVSM_MEMORYUTILIZATIONWIDGET_H

#include "utilization/UtilizationWidget.h"

class MemoryUtilizationWidget: public UtilizationWidget {
public:
    explicit MemoryUtilizationWidget(UtilizationContainer *container);

    void onDataUpdated() override;
};

#endif //QNVSM_MEMORYUTILIZATIONWIDGET_H
