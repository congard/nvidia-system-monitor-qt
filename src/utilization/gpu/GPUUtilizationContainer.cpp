#include "GPUUtilizationContainer.h"

#include <QHBoxLayout>

#include "GPUUtilizationWidget.h"
#include "Settings.h"

GPUUtilizationContainer::GPUUtilizationContainer() {
    utilizationWidget = new GPUUtilizationWidget();
    build("GPU Utilization");

    infoLabels.resize(Settings::GPUCount);

    for (uint i = 0; i < Settings::GPUCount; i++) {
        auto gpuName = new QLabel();
        gpuName->setContentsMargins(0, gpuName->fontMetrics().height() / 2, 0, 0);
        gpuName->setText("<b>GPU " + QString::number(i + 1) + "</b>");
        addWidget(gpuName);

        auto infoLayout = new QHBoxLayout();
        infoLayout->addWidget(infoLabels[i][0] = new QLabel());
        infoLayout->addWidget(infoLabels[i][1] = new QLabel());

        addLayout(infoLayout);
    }
}

void GPUUtilizationContainer::onDataUpdated() {
    for (uint i = 0; i < Settings::GPUCount; i++) {
        const auto &utilizationData = utilizationWidget->worker->udata[i];

        infoLabels[i][0]->setText(
                "Utilization: " + QString::number(utilizationData.level) + "%\n"
                "Average: " + QString::number(utilizationData.avgLevel) + "%");

        infoLabels[i][1]->setText(
                "Min: " + QString::number(utilizationData.minLevel) + "%\n"
                "Max: " + QString::number(utilizationData.maxLevel) + "%");
    }

    utilizationWidget->update();
}
