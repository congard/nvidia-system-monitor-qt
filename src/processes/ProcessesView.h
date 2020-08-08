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
    void addItem(int row, int column, const std::string &data);
    void updateItem(int row, int column, const std::string &data);
    void killProcess();
    int getRowIndexByPid(const std::string &pid);
};

#endif //QNVSM_PROCESSESVIEW_H
