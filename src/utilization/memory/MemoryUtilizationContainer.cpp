#include "MemoryUtilizationContainer.h"

#include <QHBoxLayout>

#include "Settings.h"
#include "MemoryUtilizationWorker.h"
#include "MemoryUtilizationWidget.h"

MemoryUtilizationContainer::MemoryUtilizationContainer() {
    utilizationWidget = new MemoryUtilizationWidget();
    build("Memory Utilization");

    infoLabels.resize(Settings::GPUCount);

    for (uint i = 0; i < Settings::GPUCount; i++) {
        auto gpuName = new QLabel();
        gpuName->setContentsMargins(0, gpuName->fontMetrics().height() / 2, 0, 0);
        gpuName->setText("<b>GPU " + QString::number(i + 1) + "</b>");
        addWidget(gpuName);

        auto infoLayout = new QHBoxLayout();
        infoLayout->addWidget(infoLabels[i][0] = new QLabel());
        infoLayout->addWidget(infoLabels[i][1] = new QLabel());
        infoLayout->addWidget(infoLabels[i][2] = new QLabel());
        infoLayout->addWidget(infoLabels[i][3] = new QLabel());

        addLayout(infoLayout);
    }
}

void MemoryUtilizationContainer::onDataUpdated() {
    for (uint i = 0; i < Settings::GPUCount; i++) {
        const auto &utilizationData = utilizationWidget->worker->udata[i];
        const auto &memoryData =
                reinterpret_cast<MemoryUtilizationWorker*>(utilizationWidget->worker)->memoryData[i];

        infoLabels[i][0]->setText(
                "Utilization: " + QString::number(utilizationData.level) + "%\n"
                "Average: " + QString::number(utilizationData.avgLevel) + "%");

        infoLabels[i][1]->setText(
                "Min: " + QString::number(utilizationData.minLevel) + "%\n"
                "Max: " + QString::number(utilizationData.maxLevel) + "%");

        infoLabels[i][2]->setText(
                "Total: " + QString::number(memoryData.total) + " MiB\n" +
                "Free: " + QString::number(memoryData.free) + " MiB");

        infoLabels[i][3]->setText(
                "Used: " + QString::number(memoryData.used) + " MiB");
    }

    utilizationWidget->update();
}
