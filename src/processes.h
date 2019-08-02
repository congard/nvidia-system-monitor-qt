#ifndef PROCESSES_H
#define PROCESSES_H

#include <QTableView>
#include <QAction>
#include <QMutex>
#include "worker.h"

struct ProcessList {
    std::string name;
    std::string type; // C or G
    std::string gpuIdx, pid, sm, mem, enc, dec; // integers
    
    ProcessList(const std::string &name, const std::string &type,
                const std::string &gpuIdx, const std::string &pid,
                const std::string &sm, const std::string &mem,
                const std::string &enc, const std::string &dec);
};

class ProcessesWorker : public Worker {
public:
    std::vector<ProcessList> processes;
    
    virtual void work();
    int processesIndexByPid(const std::string &pid);
};

class ProcessesTableView : public QTableView {
    Q_OBJECT
public:
    ProcessesWorker *worker;
    
    ProcessesTableView(QWidget *parent = nullptr);
    ~ProcessesTableView();
    
    void mousePressEvent(QMouseEvent *event) override;
    
private:
    std::string selectedPid = "";
    
    void killProcess();
    
public slots:
    void onDataUpdated();
};

#endif
