#ifndef QNVSM_GRAPH_H
#define QNVSM_GRAPH_H

#include <QWidget>
#include <QPainter>

#include "UtilizationWorker.h"

namespace Graph {
void drawGrid(QWidget *widget, QPainter *p);
void drawGraph(UtilizationWorker *worker, QPainter *p);
void drawStatusObjects(std::vector<QRect> &statusObjectsAreas, UtilizationData *udata, QPainter *p);
}

#endif //QNVSM_GRAPH_H
