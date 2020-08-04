#ifndef PROCESSES_H
#define PROCESSES_H

#include <QTableView>
#include <QAction>
#include <QMutex>
#include "worker.h"

struct ProcessList {
    std::string name;
    std::string type; // C or G
    std::string gpuIdx, pid, sm, mem, enc, dec, fbmem; // integers
    
    ProcessList(const std::string &name, const std::string &type,
                const std::string &gpuIdx, const std::string &pid,
                const std::string &sm, const std::string &mem,
                const std::string &enc, const std::string &dec,
                const std::string &fbmem);
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
    void _setItem(int row, int column, std::string str);
    
public slots:
    void onDataUpdated();
};

#endif
