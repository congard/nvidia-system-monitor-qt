#ifndef QNVSM_UTILIZATIONWIDGET_H
#define QNVSM_UTILIZATIONWIDGET_H

#include <QWidget>
#include <QPainter>

#include "UtilizationData.h"
#include "core/Point.h"

class UtilizationWidget: public QWidget {
public:
    UtilizationWidget();
    ~UtilizationWidget() override;

    virtual void onDataUpdated();

    void paintEvent(QPaintEvent*) override;

public:
    QVarLengthArray<UtilizationData> utilizationData;
    QVarLengthArray<std::vector<PointF>> graphPoints;

private:
    void drawGrid();
    void drawGraph();

private:
    QPainter painter;
    long lastTime {0};
};

#endif //QNVSM_UTILIZATIONWIDGET_H
