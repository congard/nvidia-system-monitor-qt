#include "GPUUtDetailsWidget.h"
#include "core/InfoProvider.h"
#include "GPUUtilizationWidget.h"
#include "../UtilizationContainer.h"
#include "../Grid.h"

#include <QLabel>

GPUUtDetailsWidget::GPUUtDetailsWidget(UtilizationContainer *container, int gpuId)
    : GPUDetailsWidget(container, gpuId)
{
    Grid grid(2);

    auto infoLayout = new QGridLayout();
    infoLayout->setSpacing(0);
    infoLayout->setHorizontalSpacing(16);

    auto addInfoLabel = [&](QLabel *&label) {
        label = new QLabel();
        infoLayout->addWidget(label, grid.row(), grid.column());
        ++grid;
    };

    QLabel *utilizationSummary {nullptr};
    QLabel *powerInfo {nullptr};
    QLabel *tempInfo {nullptr};
    QLabel *freqInfo {nullptr};

    addInfoLabel(utilizationSummary);
    utilizationSummary->setToolTip("Utilization: current (average / min / max)");

    if (InfoProvider::isPowerSupported(gpuId))
        addInfoLabel(powerInfo);

    if (InfoProvider::isGPUTempSupported(gpuId))
        addInfoLabel(tempInfo);

    if (InfoProvider::isGPUFreqSupported(gpuId)) {
        auto maxFreq = InfoProvider::getGPUFreqMax(gpuId);
        addInfoLabel(freqInfo);
        freqInfo->setToolTip(QString::asprintf("Max: %i MHz", maxFreq));
    }

    getLayout()->addLayout(infoLayout);

    connect(container->getUtilizationWidget(), &UtilizationWidget::onUpdated, this, [=]() {
        auto utilizationWidget = dynamic_cast<GPUUtilizationWidget*>(container->getUtilizationWidget());
        const auto &data = utilizationWidget->utilizationData[gpuId];

        utilizationSummary->setText(QString::asprintf("Utilization: %i%% (%i%% / %i%% / %i%%)",
            data.level, data.avgLevel, data.minLevel, data.maxLevel));

        if (tempInfo)
            tempInfo->setText(QString::asprintf("Temperature: %i °C", InfoProvider::getGPUTemp(gpuId)));

        if (powerInfo)
            powerInfo->setText(QString::asprintf("Power: %.2f W", InfoProvider::getPower(gpuId)));

        if (freqInfo) {
            freqInfo->setText(QString::asprintf("Frequency: %i MHz", InfoProvider::getGPUFreq(gpuId)));
        }
    });
}
