#ifndef QNVSM_SETTINGS_H
#define QNVSM_SETTINGS_H

#include <QColor>

#include "constants.h"

#define UPDATE_DELAY_USEC (Settings::UpdateDelay * 1000)
#define GRAPH_STEP ((float)Settings::UpdateDelay / (float)Settings::GraphLength)

namespace Settings {
extern QColor GPUColors[8];
extern uint UpdateDelay;
extern uint GraphLength;
extern int GPUCount;
}

#endif
