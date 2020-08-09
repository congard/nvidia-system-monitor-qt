#ifndef QNVSM_UTILIZATIONWIDGET_H
#define QNVSM_UTILIZATIONWIDGET_H

#include <QWidget>
#include <QPainter>

#include "UtilizationWorker.h"

class UtilizationWidget: public QWidget {
    Q_OBJECT

public:
    UtilizationWidget();
    ~UtilizationWidget() override;

    void paintEvent(QPaintEvent*) override;

public:
    UtilizationWorker *worker = nullptr;

public slots:
    void onDataUpdated();

private:
    void drawGrid();
    void drawGraph();

private:
    QPainter painter;
};

#endif //QNVSM_UTILIZATIONWIDGET_H
