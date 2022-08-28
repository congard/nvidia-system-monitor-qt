#include "SettingsManager.h"
#include "InfoProvider.h"

#include <QJsonArray>

#define constant(name) constexpr char name[] = "Graph/" #name

namespace Constants {
constant(gpuColors);
constant(updateDelay);
constant(length);
constant(smoothGraph);
}

QVarLengthArray<QColor> SettingsManager::m_gpuColors;
int SettingsManager::m_updateDelay;
int SettingsManager::m_graphLength;
bool SettingsManager::m_smoothGraph;

void SettingsManager::load() {
    using namespace Constants;

    QSettings settings(NVSM_SETTINGS);

    m_updateDelay = settings.value(updateDelay, 2000).toInt();
    m_graphLength = settings.value(length, 60000).toInt();
    m_smoothGraph = settings.value(smoothGraph, true).toBool();

    int gpuCount = InfoProvider::getGPUCount();

    // load gpu colors
    {
        auto array = settings.value(gpuColors).toJsonArray();

        m_gpuColors.resize(array.size());

        for (int i = 0; i < array.size(); i++) {
            m_gpuColors[i] = QColor(array[i].toString());
        }
    }

    if (auto gpuColorsSize = m_gpuColors.size(); gpuColorsSize < gpuCount) {
        m_gpuColors.resize(gpuCount);

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

        for (int i = gpuColorsSize; i < gpuCount; i++) {
            m_gpuColors[i] = defaultGPUColors[i % 8];
        }
    }
}

void SettingsManager::save() {
    using namespace Constants;

    QSettings settings(NVSM_SETTINGS);
    settings.setValue(updateDelay, m_updateDelay);
    settings.setValue(length, m_graphLength);
    settings.setValue(smoothGraph, m_smoothGraph);
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

void SettingsManager::setSmoothGraph(bool smoothGraph) {
    m_smoothGraph = smoothGraph;
}

void SettingsManager::setGPUColor(int index, const QColor &color) {
    m_gpuColors[index] = color;
}

const QVarLengthArray<QColor>& SettingsManager::getGPUColors() {
    return m_gpuColors;
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

bool SettingsManager::isSmoothGraph() {
    return m_smoothGraph;
}
