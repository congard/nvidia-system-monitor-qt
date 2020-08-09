#include "MemoryUtilizationWidget.h"

#include <QMouseEvent>

#include "MemoryUtilizationWorker.h"

MemoryUtilizationWidget::MemoryUtilizationWidget() {
    worker = new MemoryUtilizationWorker();
}
