#ifndef QNVSM_GPUDETAILSWIDGET_H
#define QNVSM_GPUDETAILSWIDGET_H

#include <QWidget>
#include <QVBoxLayout>

class UtilizationContainer;

class GPUDetailsWidget: public QWidget {
public:
    GPUDetailsWidget(UtilizationContainer *container, int gpuId);

    QVBoxLayout* getLayout() const;

protected:
    bool event(QEvent *event) override;
};

#endif //QNVSM_GPUDETAILSWIDGET_H
