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
        addInfoTitleLayout(i);

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
                "IO Utilization: " + QString::number(memoryData.ioUtilization) + "%\n"
                "Total: " + QString::number(memoryData.total) + " MiB");

        infoLabels[i][3]->setText(
                "Free: " + QString::number(memoryData.free) + " MiB\n"
                "Used: " + QString::number(memoryData.used) + " MiB");
    }

    utilizationWidget->update();
}
