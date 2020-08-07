#include "UtilizationWidget.h"

#include <QPainter>

#include "utilization/Graph.h"

UtilizationWidget::~UtilizationWidget() {
    delete worker;
}

void UtilizationWidget::paintEvent(QPaintEvent *) {
    QPainter p;
    p.begin(this);
    p.setRenderHint(QPainter::Antialiasing);
    Graph::drawGrid(this, &p);
    QMutexLocker locker(&worker->mutex);
    Graph::drawGraph(worker, &p);
    Graph::drawStatusObjects(statusObjectsAreas, worker->udata, &p);
    p.end();
}

void UtilizationWidget::onDataUpdated() {
    update();
}
