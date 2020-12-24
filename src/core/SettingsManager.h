#ifndef QNVSM_SETTINGSMANAGER_H
#define QNVSM_SETTINGSMANAGER_H

#include <QVarLengthArray>
#include <QSettings>
#include <QColor>

#define NVSM_SETTINGS "congard", "NVSM"

class SettingsManager {
public:
    static void init();
    static void load();
    static void save();

    static void setGPUColors(const QVarLengthArray<QColor> &gpuColors);
    static void setUpdateDelay(uint updateDelay);
    static void setGraphLength(uint graphLength);

    static void setGPUColor(uint index, const QColor &color);

    static const QVarLengthArray<QString>& getGPUNames();
    static const QVarLengthArray<QColor>& getGPUColors();

    static const QString& getGPUName(uint index);
    static const QColor& getGPUColor(uint index);

    static uint getUpdateDelay();
    static uint getGraphLength();
    static uint getGPUCount();

private:
    static QVarLengthArray<QString> m_gpuNames;
    static QVarLengthArray<QColor> m_gpuColors;
    static uint m_updateDelay;
    static uint m_graphLength;
    static uint m_gpuCount;
};

#endif //QNVSM_SETTINGSMANAGER_H
