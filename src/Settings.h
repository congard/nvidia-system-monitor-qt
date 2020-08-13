#ifndef QNVSM_SETTINGS_H
#define QNVSM_SETTINGS_H

#include <QColor>
#include <QVector>

#include "constants.h"

#define UPDATE_DELAY_USEC (Settings::UpdateDelay * 1000)
#define GRAPH_STEP ((float)Settings::UpdateDelay / (float)Settings::GraphLength)

namespace Settings {
extern QVector<QColor> GPUColors;
extern uint UpdateDelay;
extern uint GraphLength;
extern int GPUCount;

void load();
}

#endif
