#include "GPUUtilizationContainer.h"

#include <QGridLayout>

#include "GPUUtilizationWidget.h"

#include "core/SettingsManager.h"
#include "core/InfoProvider.h"
#include "utilization/Grid.h"

enum {
    power = 1,
    temp,
    freq
};

GPUUtilizationContainer::GPUUtilizationContainer() {
    utilizationWidget = new GPUUtilizationWidget();
    build("GPU Utilization");

    Grid grid(2);

    for (int i = 0; i < InfoProvider::getGPUCount(); i++) {
        addInfoTitleLayout(i);

        auto infoLayout = new QGridLayout();
        infoLayout->setSpacing(0);

        auto addInfoLabel = [&](int id) {
            infoLayout->addWidget(getInfoLabel(i, id), grid.row(), grid.column());
            ++grid;
        };

        addInfoLabel(UtInfoLabelId);

        if (InfoProvider::isPowerSupported(i))
            addInfoLabel(power);

        if (InfoProvider::isGPUTempSupported(i))
            addInfoLabel(temp);

        if (InfoProvider::isGPUFreqSupported(i))
            addInfoLabel(freq);

        getLayout()->addLayout(infoLayout);
    }

    updateData();

    setMouseTracking(true);
}

void GPUUtilizationContainer::onDataUpdated() {
    for (int i = 0; i < InfoProvider::getGPUCount(); i++) {
        const auto &data = utilizationWidget->worker->udata[i];

        auto infoLabel = [&](int index) {
            return findChild<QLabel*>(getInfoLabelName(i, index));
        };

        infoLabel(0)->setText(QString::asprintf("Utilization: %i%% (%i%% / %i%% / %i%%)",
                                                data.level, data.avgLevel, data.minLevel, data.maxLevel));

        if (InfoProvider::isGPUTempSupported(i)) {
            infoLabel(temp)->setText(QString::asprintf("Temperature: %i °C", InfoProvider::getGPUTemp(i)));
        }

        if (InfoProvider::isPowerSupported(i)) {
            infoLabel(power)->setText(QString::asprintf("Power: %.2f W", InfoProvider::getPower(i)));
        }

        if (InfoProvider::isGPUFreqSupported(i)) {
            infoLabel(freq)->setText(QString::asprintf("Frequency: %i MHz", InfoProvider::getGPUFreq(i)));
        }
    }

    utilizationWidget->update();
}
