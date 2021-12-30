#include "SettingsManager.h"
#include "NVSMIParser.h"

#include <QJsonArray>

#define constant(name) constexpr char name[] = "Graph/" #name

namespace Constants {
constant(gpuColors);
constant(updateDelay);
constant(length);
}

QVarLengthArray<QString> SettingsManager::m_gpuNames;
QVarLengthArray<QColor> SettingsManager::m_gpuColors;
int SettingsManager::m_updateDelay;
int SettingsManager::m_graphLength;
int SettingsManager::m_gpuCount;

void SettingsManager::init() {
    NVSMIParser::init();
    m_gpuCount = NVSMIParser::getGPUCount();
    m_gpuNames = NVSMIParser::getGPUNames();
}

void SettingsManager::load() {
    using namespace Constants;

    QSettings settings(NVSM_SETTINGS);

    m_updateDelay = settings.value(updateDelay, 2000).toInt();
    m_graphLength = settings.value(length, 60000).toInt();

    // load gpu colors
    {
        auto array = settings.value(gpuColors).toJsonArray();

        m_gpuColors.resize(array.size());

        for (int i = 0; i < array.size(); i++) {
            m_gpuColors[i] = QColor(array[i].toString());
        }
    }

    if (auto gpuColorsSize = m_gpuColors.size(); gpuColorsSize < m_gpuCount) {
        m_gpuColors.resize(m_gpuCount);

        QColor defaultGPUColors[8] = {
            {0, 255, 0},
            {0, 0, 255},
            {255, 0, 0},
            {255, 255, 0},
            {255, 0, 255},
            {0, 255, 255},
            {255, 255, 255},
            {32, 32, 32}
        };

        for (int i = gpuColorsSize; i < m_gpuCount; i++) {
            m_gpuColors[i] = defaultGPUColors[i % 8];
        }
    }
}

void SettingsManager::save() {
    using namespace Constants;

    QSettings settings(NVSM_SETTINGS);
    settings.setValue(updateDelay, m_updateDelay);
    settings.setValue(length, m_graphLength);
    settings.setValue(gpuColors, [&]() {
        QJsonArray array;

        for (const auto &color : m_gpuColors)
            array.append(color.name());

        return array;
    }());
}

void SettingsManager::setGPUColors(const QVarLengthArray<QColor> &gpuColors) {
    m_gpuColors = gpuColors;
}

void SettingsManager::setUpdateDelay(int updateDelay) {
    m_updateDelay = updateDelay;
}

void SettingsManager::setGraphLength(int graphLength) {
    m_graphLength = graphLength;
}

void SettingsManager::setGPUColor(int index, const QColor &color) {
    m_gpuColors[index] = color;
}

const QVarLengthArray<QString>& SettingsManager::getGPUNames() {
    return m_gpuNames;
}

const QVarLengthArray<QColor>& SettingsManager::getGPUColors() {
    return m_gpuColors;
}

const QString& SettingsManager::getGPUName(int index) {
    return m_gpuNames[index];
}

const QColor& SettingsManager::getGPUColor(int index) {
    return m_gpuColors[index];
}

int SettingsManager::getUpdateDelay() {
    return m_updateDelay;
}

int SettingsManager::getGraphLength() {
    return m_graphLength;
}

int SettingsManager::getGPUCount() {
    return m_gpuCount;
}
