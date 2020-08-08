#ifndef QNVSM_PROCESSESVIEW_H
#define QNVSM_PROCESSESVIEW_H

#include <QTreeView>
#include "ProcessesWorker.h"

class ProcessesView: public QTreeView {
    Q_OBJECT

public:
    explicit ProcessesView(QWidget *parent = nullptr);
    ~ProcessesView() override;

    void mousePressEvent(QMouseEvent *event) override;

public:
    ProcessesWorker *worker;

public slots:
    void onDataUpdated();

private:
    std::string selectedPid;

private:
    void killProcess();
};

#endif //QNVSM_PROCESSESVIEW_H
