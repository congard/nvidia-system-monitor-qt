#ifndef QNVSM_UTILIZATIONWIDGET_H
#define QNVSM_UTILIZATIONWIDGET_H

#include <QWidget>
#include <QPainter>

#include "UtilizationWorker.h"

// TODO: remove status objects, do not draw text: use widgets instead, UtilizationWidget - just Graph
// TODO: create namespace NVSMIParser

class UtilizationWidget: public QWidget {
    Q_OBJECT

public:
    ~UtilizationWidget() override;

    void paintEvent(QPaintEvent*) override;

    static void init();

public:
    std::vector<QRect> statusObjectsAreas;
    UtilizationWorker *worker;

public slots:
    void onDataUpdated();

private:
    int graphStartY, graphEndY, width; // tmp

    void drawGrid();
    void drawGraph();
    void drawStatusObjects();

private:
    QPainter painter;

private:
    static QString fullUtilizationText;
    static QString noUtilizationText;
    static QString graphStepSecondsText;
    static QString graphSecondsText;
    static int fullUtilizationTextWidth;
    static int noUtilizationTextWidth;
    static int textHeight;
};

#endif //QNVSM_UTILIZATIONWIDGET_H
