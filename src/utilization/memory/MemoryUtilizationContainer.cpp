#include "MemoryUtilizationContainer.h"

#include <QHBoxLayout>

#include "MemoryUtilizationWorker.h"
#include "MemoryUtilizationWidget.h"

#include "core/SettingsManager.h"
#include "core/InfoProvider.h"

MemoryUtilizationContainer::MemoryUtilizationContainer() {
    utilizationWidget = new MemoryUtilizationWidget();
    build("Memory Utilization");

    for (int i = 0; i < SettingsManager::getGPUCount(); i++) {
        addInfoTitleLayout(i);

        auto infoLayout = new QHBoxLayout();
        infoLayout->addWidget(getInfoLabel(i, 0));
        infoLayout->addWidget(getInfoLabel(i, 1));
        infoLayout->addWidget(getInfoLabel(i, 2));

        getLayout()->addLayout(infoLayout);
    }

    updateData();
}

void MemoryUtilizationContainer::onDataUpdated() {
    for (int i = 0; i < SettingsManager::getGPUCount(); i++) {
        const auto &utilizationData = utilizationWidget->worker->udata[i];
        const auto &memoryData =
                reinterpret_cast<MemoryUtilizationWorker*>(utilizationWidget->worker)->memoryData[i];

        auto infoLabel = [&](int gpuIndex, int index) {
            return findChild<QLabel *>(getInfoLabelName(gpuIndex, index));
        };

        infoLabel(i, UtInfoLabelId)->setText(
            QString::asprintf("Utilization: %i%% (%i%% / %i%% / %i%%)\nIO Utilization: %i%%",
                utilizationData.level, utilizationData.avgLevel, utilizationData.minLevel, utilizationData.maxLevel,
                memoryData.ioUtilization
            )
        );

        infoLabel(i, 1)->setText(QString::asprintf("Total: %i MiB\nFree: %i MiB", memoryData.total, memoryData.free));

        auto label2 = QString::asprintf("Used: %i MiB", memoryData.used);

        if (InfoProvider::isMemTempSupported(i)) {
            label2.append(QString::asprintf("\nTemperature: %i °C", InfoProvider::getMemTemp(i)));
        }

        infoLabel(i, 2)->setText(label2);
    }

    utilizationWidget->update();
}
