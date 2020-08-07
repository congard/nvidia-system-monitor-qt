#include "GPUUtilizationWidget.h"

#include <QMouseEvent>
#include <QToolTip>

#include "GPUUtilizationWorker.h"

GPUUtilizationWidget::GPUUtilizationWidget() {
    worker = new GPUUtilizationWorker();
    setMouseTracking(true);
}

void GPUUtilizationWidget::mouseMoveEvent(QMouseEvent* event) {
    for (size_t i = 0; i < statusObjectsAreas.size(); i++) {
        const auto &area = statusObjectsAreas[i];

        if ((area.x() <= event->x()) && (area.x() + area.width() >= event->x()) &&
            (area.y() <= event->y()) && (area.y() + area.height() >= event->y()))
        {
            QToolTip::showText(
                    event->globalPos(),
                    "GPU Utilization: " + QString::number(worker->udata[i].level) +
                    "\nAverage: " + QString::number(worker->udata[i].avgLevel) +
                    "\nMin: " + QString::number(worker->udata[i].minLevel) +
                    "\nMax: " + QString::number(worker->udata[i].maxLevel)
            );

            return;
        }
    }
}
