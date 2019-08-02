#ifndef SETTINGS_H
#define SETTINGS_H

#include "constants.h"
#include <QColor>

extern uint UPDATE_DELAY;
extern uint GRAPH_LENGTH;
extern int GPU_COUNT;

#define UPDATE_DELAY_USEC (UPDATE_DELAY * 1000)
#define GRAPH_STEP ((float)UPDATE_DELAY / (float)GRAPH_LENGTH)

#define _c(r, g, b) QColor(r, g, b)

extern QColor gpuColors[8];

#endif
