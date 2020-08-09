#include "GPUUtilizationWidget.h"

#include <QMouseEvent>

#include "GPUUtilizationWorker.h"

GPUUtilizationWidget::GPUUtilizationWidget() {
    worker = new GPUUtilizationWorker();
}
