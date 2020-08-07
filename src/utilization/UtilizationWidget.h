#ifndef QNVSM_UTILIZATIONWIDGET_H
#define QNVSM_UTILIZATIONWIDGET_H

#include <QWidget>

#include "UtilizationWorker.h"

class UtilizationWidget: public QWidget {
    Q_OBJECT

public:
    ~UtilizationWidget() override;

    void paintEvent(QPaintEvent*) override;

public:
    std::vector<QRect> statusObjectsAreas;
    UtilizationWorker *worker;

public slots:
    void onDataUpdated();
};

#endif //QNVSM_UTILIZATIONWIDGET_H
