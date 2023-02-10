#include "GPUMemDetailsWidget.h"
#include "core/InfoProvider.h"
#include "MemoryUtilizationWidget.h"
#include "../UtilizationContainer.h"
#include "../Grid.h"

#include <QLabel>

GPUMemDetailsWidget::GPUMemDetailsWidget(UtilizationContainer *container, int gpuId)
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
    QLabel *ioUtilizationInfo {nullptr};
    QLabel *totalMemInfo {nullptr};
    QLabel *freeMemInfo {nullptr};
    QLabel *usedMemInfo {nullptr};
    QLabel *memTempInfo {nullptr};
    QLabel *memFreqInfo {nullptr};

    addInfoLabel(utilizationSummary);
    utilizationSummary->setToolTip("Utilization: current (average / min / max)");

    addInfoLabel(ioUtilizationInfo);
    addInfoLabel(totalMemInfo);
    addInfoLabel(freeMemInfo);
    addInfoLabel(usedMemInfo);

    if (InfoProvider::isMemTempSupported(gpuId))
        addInfoLabel(memTempInfo);

    addInfoLabel(memFreqInfo);
    memFreqInfo->setToolTip(QString::asprintf("Max: %i MHz", InfoProvider::getMemFreqMax(gpuId)));

    getLayout()->addLayout(infoLayout);

    connect(container->getUtilizationWidget(), &UtilizationWidget::onUpdated, this, [=]() {
        auto utilizationWidget = dynamic_cast<MemoryUtilizationWidget*>(container->getUtilizationWidget());
        const auto &data = utilizationWidget->utilizationData[gpuId];

        utilizationSummary->setText(QString::asprintf("Utilization: %i%% (%i%% / %i%% / %i%%)",
            data.level, data.avgLevel, data.minLevel, data.maxLevel));

        ioUtilizationInfo->setText(QString::asprintf("IO Utilization: %i%%", InfoProvider::getMemUtil(gpuId)));

        totalMemInfo->setText(QString::asprintf("Total: %i MiB", InfoProvider::getMemTotal(gpuId)));
        freeMemInfo->setText(QString::asprintf("Free: %i MiB", InfoProvider::getMemFree(gpuId)));
        usedMemInfo->setText(QString::asprintf("Used: %i MiB", InfoProvider::getMemUsed(gpuId)));

        if (memTempInfo)
            memTempInfo->setText(QString::asprintf("Temperature: %i °C", InfoProvider::getMemTemp(gpuId)));

        if (memFreqInfo) {
            memFreqInfo->setText(QString::asprintf("Frequency: %i MHz", InfoProvider::getMemFreq(gpuId)));
        }
    });
}
