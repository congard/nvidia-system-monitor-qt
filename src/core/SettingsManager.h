#ifndef QNVSM_SETTINGSMANAGER_H
#define QNVSM_SETTINGSMANAGER_H

#include <QVarLengthArray>
#include <QSettings>
#include <QColor>

#define NVSM_SETTINGS "congard", "NVSM"

class SettingsManager {
public:
    static void load();
    static void save();

    static void setGPUColors(const QVarLengthArray<QColor> &gpuColors);
    static void setUpdateDelay(int updateDelay);
    static void setGraphLength(int graphLength);

    static void setGPUColor(int index, const QColor &color);

    static const QVarLengthArray<QColor>& getGPUColors();
    static const QColor& getGPUColor(int index);

    static int getUpdateDelay();
    static int getGraphLength();

private:
    static QVarLengthArray<QColor> m_gpuColors;
    static int m_updateDelay;
    static int m_graphLength;
};

#endif //QNVSM_SETTINGSMANAGER_H
