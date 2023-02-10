#ifndef QNVSM_UTILIZATIONCONTAINER_H
#define QNVSM_UTILIZATIONCONTAINER_H

#include "UtilizationWidget.h"
#include "FlowLayout.h"

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
    FlowLayout* getDescLayout();

protected:
    UtilizationWidget *utilizationWidget;
    FlowLayout *m_descLayout;
};

#endif //QNVSM_UTILIZATIONCONTAINER_H
