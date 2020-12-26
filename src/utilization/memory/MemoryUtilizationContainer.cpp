#include "MemoryUtilizationContainer.h"

#include <QHBoxLayout>

#include "MemoryUtilizationWorker.h"
#include "MemoryUtilizationWidget.h"

#include "core/SettingsManager.h"
#include "utilization/UtilizationContainerCommon.h"

MemoryUtilizationContainer::MemoryUtilizationContainer() {
    utilizationWidget = new MemoryUtilizationWidget();
    build("Memory Utilization");

    for (uint i = 0; i < SettingsManager::getGPUCount(); i++) {
        addInfoTitleLayout(i);

        auto infoLayout = new QHBoxLayout();
        infoLayout->addWidget(getInfoLabel(i, 0));
        infoLayout->addWidget(getInfoLabel(i, 1));
        infoLayout->addWidget(getInfoLabel(i, 2));
        infoLayout->addWidget(getInfoLabel(i, 3));

        getLayout()->addLayout(infoLayout);
    }

    updateData();
}

void MemoryUtilizationContainer::onDataUpdated() {
    for (uint i = 0; i < SettingsManager::getGPUCount(); i++) {
        const auto &utilizationData = utilizationWidget->worker->udata[i];
        const auto &memoryData =
                reinterpret_cast<MemoryUtilizationWorker*>(utilizationWidget->worker)->memoryData[i];

        auto infoLabel = [&](uint gpuIndex, uint index) {
            return findChild<QLabel *>(getInfoLabelName(gpuIndex, index));
        };

        infoLabel(i, 0)->setText(
            "Utilization: " + QString::number(utilizationData.level) + "%\n"
            "Average: " + QString::number(utilizationData.avgLevel) + "%"
        );

        infoLabel(i, 1)->setText(
            "Min: " + QString::number(utilizationData.minLevel) + "%\n"
            "Max: " + QString::number(utilizationData.maxLevel) + "%"
        );

        infoLabel(i, 2)->setText(
            "IO Utilization: " + QString::number(memoryData.ioUtilization) + "%\n"
            "Total: " + QString::number(memoryData.total) + " MiB"
        );

        infoLabel(i, 3)->setText(
            "Free: " + QString::number(memoryData.free) + " MiB\n"
            "Used: " + QString::number(memoryData.used) + " MiB"
        );
    }

    utilizationWidget->update();
}
