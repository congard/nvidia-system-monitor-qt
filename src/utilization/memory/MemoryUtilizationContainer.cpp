#include "MemoryUtilizationContainer.h"

#include <QGridLayout>

#include "MemoryUtilizationWorker.h"
#include "MemoryUtilizationWidget.h"

#include "core/SettingsManager.h"
#include "core/InfoProvider.h"
#include "utilization/Grid.h"

enum {
    ioUtilization = 1,
    totalMem,
    freeMem,
    usedMem,
    memTemp,
    memFreq
};

MemoryUtilizationContainer::MemoryUtilizationContainer() {
    utilizationWidget = new MemoryUtilizationWidget();
    build("Memory Utilization");

    Grid grid(2);

    for (int i = 0; i < SettingsManager::getGPUCount(); i++) {
        addInfoTitleLayout(i);

        auto infoLayout = new QGridLayout();
        infoLayout->setSpacing(0);

        auto addInfoLabel = [&](int id) {
            infoLayout->addWidget(getInfoLabel(i, id), grid.row(), grid.column());
            ++grid;
        };

        addInfoLabel(UtInfoLabelId);
        addInfoLabel(ioUtilization);
        addInfoLabel(totalMem);
        addInfoLabel(freeMem);
        addInfoLabel(usedMem);

        if (InfoProvider::isMemTempSupported(i))
            addInfoLabel(memTemp);

        addInfoLabel(memFreq);

        getLayout()->addLayout(infoLayout);
    }

    updateData();
}

void MemoryUtilizationContainer::onDataUpdated() {
    for (int i = 0; i < SettingsManager::getGPUCount(); i++) {
        const auto &utilizationData = utilizationWidget->worker->udata[i];
        const auto &memoryData =
                reinterpret_cast<MemoryUtilizationWorker*>(utilizationWidget->worker)->memoryData[i];

        auto infoLabel = [&](int index) {
            return findChild<QLabel*>(getInfoLabelName(i, index));
        };

        infoLabel(UtInfoLabelId)->setText(
            QString::asprintf("Utilization: %i%% (%i%% / %i%% / %i%%)",
                utilizationData.level, utilizationData.avgLevel, utilizationData.minLevel, utilizationData.maxLevel
            )
        );

        infoLabel(ioUtilization)->setText(QString::asprintf("IO Utilization: %i%%", memoryData.ioUtilization));

        infoLabel(totalMem)->setText(QString::asprintf("Total: %i MiB", memoryData.total));
        infoLabel(freeMem)->setText(QString::asprintf("Free: %i MiB", memoryData.free));
        infoLabel(usedMem)->setText(QString::asprintf("Used: %i MiB", memoryData.used));

        if (InfoProvider::isMemTempSupported(i)) {
            infoLabel(memTemp)->setText(QString::asprintf("Temperature: %i °C", InfoProvider::getMemTemp(i)));
        }

        if (InfoProvider::isMemFreqSupported(i)) {
            infoLabel(memFreq)->setText(QString::asprintf("Frequency: %i MHz", InfoProvider::getMemFreq(i)));
        }
    }

    utilizationWidget->update();
}
