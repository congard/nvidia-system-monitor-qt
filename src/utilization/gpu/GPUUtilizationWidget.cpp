#include "GPUUtilizationWidget.h"

#include "core/InfoProvider.h"

GPUUtilizationWidget::GPUUtilizationWidget(UtilizationContainer *container)
    : UtilizationWidget(container) {}

void GPUUtilizationWidget::onDataUpdated() {
    for (int i = 0; i < InfoProvider::getGPUCount(); i++) {
        utilizationData[i].level = InfoProvider::getGPUUtil(i);
    }

    UtilizationWidget::onDataUpdated();
}
