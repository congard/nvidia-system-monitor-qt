#include "MemoryUtilizationWidget.h"

#include "core/InfoProvider.h"

MemoryUtilizationWidget::MemoryUtilizationWidget(UtilizationContainer *container)
    : UtilizationWidget(container) {}

void MemoryUtilizationWidget::onDataUpdated() {
    for (int i = 0; i < InfoProvider::getGPUCount(); i++) {
        utilizationData[i].level = InfoProvider::getMemUsed(i) * 100 / InfoProvider::getMemTotal(i);
    }

    UtilizationWidget::onDataUpdated();
}
