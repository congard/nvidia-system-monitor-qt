#ifndef QNVSM_UTILIZATIONCONTAINER_H
#define QNVSM_UTILIZATIONCONTAINER_H

#include <QLabel>

#include "UtilizationWidget.h"

class QVBoxLayout;

class UtilizationContainer: public QWidget {
public:
    constexpr static int UtInfoLabelId = 0;

public:
    void build(const QString &name);

    void updateData();

    UtilizationWidget* getUtilizationWidget() const;

protected:
    bool event(QEvent *event) override;
    void addInfoTitleLayout(int gpuIndex);
    QVBoxLayout* getLayout();

    inline static QString getInfoLabelName(int gpuIndex, int index) {
        return "il_" + QString::number(gpuIndex) + "_" + QString::number(index);
    }

    inline static QLabel* getInfoLabel(int gpuIndex, int index) {
        auto label = new QLabel();
        label->setObjectName(getInfoLabelName(gpuIndex, index));
        return label;
    }

protected:
    UtilizationWidget *utilizationWidget;
};

#endif //QNVSM_UTILIZATIONCONTAINER_H
