#include "GPUUtilizationContainer.h"

#include <QHBoxLayout>

#include "GPUUtilizationWidget.h"

#include "core/SettingsManager.h"
#include "utilization/UtilizationContainerCommon.h"

GPUUtilizationContainer::GPUUtilizationContainer() {
    utilizationWidget = new GPUUtilizationWidget();
    build("GPU Utilization");

    for (uint i = 0; i < SettingsManager::getGPUCount(); i++) {
        addInfoTitleLayout(i);

        auto infoLayout = new QHBoxLayout();
        infoLayout->addWidget(getInfoLabel(i, 0));
        infoLayout->addWidget(getInfoLabel(i, 1));

        getLayout()->addLayout(infoLayout);
    }

    updateData();
}

void GPUUtilizationContainer::onDataUpdated() {
    for (uint i = 0; i < SettingsManager::getGPUCount(); i++) {
        const auto &utilizationData = utilizationWidget->worker->udata[i];

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
    }

    utilizationWidget->update();
}
