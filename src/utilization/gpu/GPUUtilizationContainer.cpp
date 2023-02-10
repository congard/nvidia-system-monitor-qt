#include "GPUUtilizationContainer.h"

#include "GPUUtilizationWidget.h"
#include "GPUUtDetailsWidget.h"

#include "core/InfoProvider.h"

GPUUtilizationContainer::GPUUtilizationContainer() {
    utilizationWidget = new GPUUtilizationWidget(this);
    build("GPU Utilization");

    for (int i = 0; i < InfoProvider::getGPUCount(); i++)
        getDescLayout()->addWidget(new GPUUtDetailsWidget(this, i));

    updateLegend();
}
