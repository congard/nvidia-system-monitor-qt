#ifndef QNVSM_UTILIZATIONCONTAINER_H
#define QNVSM_UTILIZATIONCONTAINER_H

#include "UtilizationWidget.h"

class QVBoxLayout;

class UtilizationContainer: public QWidget {
public:
    void build(const QString &name);

    void updateData();

    UtilizationWorker* getWorker();

protected:
    void addInfoTitleLayout(int gpuIndex);
    QVBoxLayout* getLayout();

protected:
    UtilizationWidget *utilizationWidget;
};

#endif //QNVSM_UTILIZATIONCONTAINER_H
