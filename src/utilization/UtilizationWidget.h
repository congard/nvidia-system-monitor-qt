#ifndef QNVSM_UTILIZATIONWIDGET_H
#define QNVSM_UTILIZATIONWIDGET_H

#include <QWidget>
#include <QPainter>

#include "UtilizationData.h"
#include "core/Point.h"

class UtilizationContainer;

class UtilizationWidget: public QWidget {
    Q_OBJECT

public:
    explicit UtilizationWidget(UtilizationContainer *container);
    ~UtilizationWidget() override;

    void paintEvent(QPaintEvent*) override;

signals:
    void onUpdated();

public:
    QVarLengthArray<UtilizationData> utilizationData;
    QVarLengthArray<std::vector<PointF>> graphPoints;

protected slots:
    virtual void onDataUpdated();

private:
    void drawGrid();
    void drawGraph();

private:
    QPainter painter;
    long lastTime {0};
};

#endif //QNVSM_UTILIZATIONWIDGET_H
