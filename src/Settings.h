#ifndef QNVSM_SETTINGS_H
#define QNVSM_SETTINGS_H

#include <QString>
#include <QColor>
#include <QVarLengthArray>

#include "constants.h"

#define UPDATE_DELAY_USEC (Settings::UpdateDelay * 1000)
#define GRAPH_STEP ((float)Settings::UpdateDelay / (float)Settings::GraphLength)

namespace Settings {
extern QVarLengthArray<QColor> GPUColors;
extern QVarLengthArray<QString> GPUNames;
extern uint UpdateDelay;
extern uint GraphLength;
extern int GPUCount;

void load();
}

#endif
