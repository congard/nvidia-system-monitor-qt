#ifndef PROCESSES_H
#define PROCESSES_H

#include <QTableView>
#include <QAction>
#include <QMutex>
#include "worker.h"

struct ProcessList {
    std::string name;
    std::string type; // C or G
    std::string gpuIdx, pid, mem; // integers
    
    ProcessList(const std::string &name, const std::string &type,
                const std::string &gpuIdx, const std::string &pid,
                const std::string &mem);
};

class ProcessesWorker : public Worker {
public:
    std::vector<ProcessList> processes;
    
    void work() override;
    int processesIndexByPid(const std::string &pid);
};

class ProcessesTableView : public QTableView {
    Q_OBJECT
public:
    ProcessesWorker *worker;
    
    explicit ProcessesTableView(QWidget *parent = nullptr);
    ~ProcessesTableView() override;
    
    void mousePressEvent(QMouseEvent *event) override;
    
private:
    std::string selectedPid = "";
    
    void killProcess();
    
public slots:
    void onDataUpdated();
};

#endif
