#ifndef QNVSM_PROCESSESTABLEVIEW_H
#define QNVSM_PROCESSESTABLEVIEW_H

#include <QTableView>
#include "ProcessesWorker.h"

class ProcessesTableView: public QTableView {
    Q_OBJECT

public:
    explicit ProcessesTableView(QWidget *parent = nullptr);
    ~ProcessesTableView() override;

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

#endif //QNVSM_PROCESSESTABLEVIEW_H
