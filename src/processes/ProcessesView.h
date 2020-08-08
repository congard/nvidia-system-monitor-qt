#ifndef QNVSM_PROCESSESVIEW_H
#define QNVSM_PROCESSESVIEW_H

#include <QTableView>
#include "ProcessesWorker.h"

class ProcessesView: public QTableView {
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
