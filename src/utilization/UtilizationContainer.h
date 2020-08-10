#ifndef QNVSM_UTILIZATIONCONTAINER_H
#define QNVSM_UTILIZATIONCONTAINER_H

#include <QVBoxLayout>

#include "UtilizationWidget.h"

class UtilizationContainer: public QVBoxLayout {
public:
    void build(const QString &name);
    QWidget* getWidget();
    UtilizationWorker* getWorker();

protected:
    UtilizationWidget *utilizationWidget;
};

#endif //QNVSM_UTILIZATIONCONTAINER_H
