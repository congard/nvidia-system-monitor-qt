#ifndef QNVSM_UTILIZATIONCONTAINER_H
#define QNVSM_UTILIZATIONCONTAINER_H

#include "UtilizationWidget.h"
#include "FlowGridLayout.h"

class QVBoxLayout;

class UtilizationContainer: public QWidget {
    Q_OBJECT

public:
    constexpr static int UtInfoLabelId = 0;

public:
    void build(const QString &name);

    void updateLegend();

    UtilizationWidget* getUtilizationWidget() const;

public slots:
    void dataUpdated();

signals:
    void onLegendUpdate();
    void onDataUpdated();

protected:
    FlowGridLayout* getDescLayout();

protected:
    UtilizationWidget *utilizationWidget;
    FlowGridLayout *m_descLayout;
};

#endif //QNVSM_UTILIZATIONCONTAINER_H
