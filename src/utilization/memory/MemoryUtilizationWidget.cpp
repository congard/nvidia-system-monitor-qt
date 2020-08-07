#include "MemoryUtilizationWidget.h"

#include <QMouseEvent>
#include <QToolTip>

#include "MemoryUtilizationWorker.h"

MemoryUtilizationWidget::MemoryUtilizationWidget() {
    worker = new MemoryUtilizationWorker;
    setMouseTracking(true);
}

void MemoryUtilizationWidget::mouseMoveEvent(QMouseEvent* event) {
    for (size_t i = 0; i < statusObjectsAreas.size(); i++) {
        const auto &area = statusObjectsAreas[i];

        if ((area.x() <= event->x()) && (area.x() + area.width() >= event->x()) &&
            (area.y() <= event->y()) && (area.y() + area.height() >= event->y()))
        {
            QToolTip::showText(
                    event->globalPos(),
                    "Memory Utilization: " + QString::number(worker->udata[i].level) +
                    "\nAverage: " + QString::number(worker->udata[i].avgLevel) +
                    "\nMin: " + QString::number(worker->udata[i].minLevel) +
                    "\nMax: " + QString::number(worker->udata[i].maxLevel) +
                    "\nTotal: " + QString::number(((MemoryUtilizationWorker*)worker)->memoryData[i].total) + " MiB" +
                    "\nFree: " + QString::number(((MemoryUtilizationWorker*)worker)->memoryData[i].free) + " MiB" +
                    "\nUsed: " + QString::number(((MemoryUtilizationWorker*)worker)->memoryData[i].used) + " MiB"
            );

            return;
        }
    }
}
