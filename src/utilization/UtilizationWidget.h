#ifndef QNVSM_UTILIZATIONWIDGET_H
#define QNVSM_UTILIZATIONWIDGET_H

#include <QWidget>
#include <QPainter>

#include "UtilizationWorker.h"

class UtilizationWidget: public QWidget {
public:
    UtilizationWidget();
    ~UtilizationWidget() override;

    void paintEvent(QPaintEvent*) override;

public:
    UtilizationWorker *worker = nullptr;

private:
    void drawGrid();
    void drawGraph();

private:
    QPainter painter;
};

#endif //QNVSM_UTILIZATIONWIDGET_H
