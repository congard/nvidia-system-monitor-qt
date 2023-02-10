#include "MemoryUtilizationContainer.h"

#include "MemoryUtilizationWidget.h"
#include "GPUMemDetailsWidget.h"

#include "core/InfoProvider.h"

MemoryUtilizationContainer::MemoryUtilizationContainer() {
    utilizationWidget = new MemoryUtilizationWidget(this);
    build("Memory Utilization");

    for (int i = 0; i < InfoProvider::getGPUCount(); i++)
        getDescLayout()->addWidget(new GPUMemDetailsWidget(this, i));

    updateLegend();
}
