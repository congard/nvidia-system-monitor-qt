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
uint SettingsManager::m_updateDelay;
uint SettingsManager::m_graphLength;
uint SettingsManager::m_gpuCount;

void SettingsManager::init() {
    NVSMIParser::init();
    m_gpuCount = NVSMIParser::getGPUCount();
    m_gpuNames = NVSMIParser::getGPUNames();
}

void SettingsManager::load() {
    using namespace Constants;

    QSettings settings(NVSM_SETTINGS);

    m_updateDelay = settings.value(updateDelay, 2000).toUInt();
    m_graphLength = settings.value(length, 60000).toUInt();

    // load gpu colors
    {
        auto array = settings.value(gpuColors).toJsonArray();

        m_gpuColors.resize(array.size());

        for (uint i = 0; i < array.size(); i++) {
            m_gpuColors[i] = QColor(array[i].toInt());
        }
    }

    if (m_gpuColors.size() < m_gpuCount) {
        auto gpuColorsSize = m_gpuColors.size();

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

        for (uint i = gpuColorsSize; i < m_gpuCount; i++) {
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
            array.append(color.value());

        return array;
    }());
}

void SettingsManager::setGPUColors(const QVarLengthArray<QColor> &gpuColors) {
    m_gpuColors = gpuColors;
}

void SettingsManager::setUpdateDelay(uint updateDelay) {
    m_updateDelay = updateDelay;
}

void SettingsManager::setGraphLength(uint graphLength) {
    m_graphLength = graphLength;
}

void SettingsManager::setGPUColor(uint index, const QColor &color) {
    m_gpuColors[index] = color;
}

const QVarLengthArray<QString>& SettingsManager::getGPUNames() {
    return m_gpuNames;
}

const QVarLengthArray<QColor>& SettingsManager::getGPUColors() {
    return m_gpuColors;
}

const QString& SettingsManager::getGPUName(uint index) {
    return m_gpuNames[index];
}

const QColor& SettingsManager::getGPUColor(uint index) {
    return m_gpuColors[index];
}

uint SettingsManager::getUpdateDelay() {
    return m_updateDelay;
}

uint SettingsManager::getGraphLength() {
    return m_graphLength;
}

uint SettingsManager::getGPUCount() {
    return m_gpuCount;
}
