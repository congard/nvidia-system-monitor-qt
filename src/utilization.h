#ifndef UTILIZATION_H
#define UTILIZATION_H

#include <QWidget>
#include <QPainter>
#include <vector>
#include <QMutex>

#include "constants.h"
#include "worker.h"

struct Point {
    float x; // [0; 1]
    int y; // [0; 100]
    
    Point(const float x, const int y);
};

struct UtilizationData {
    int level = 0; // current graph point in percents
    int avgLevel = 0, minLevel = 0, maxLevel = 0;
};

struct MemoryData {
    int total, free, used;
};

class UtilizationWorker : public Worker {
public:
    std::vector<Point> *gpoints; // graph points
    UtilizationData *udata;

    UtilizationWorker();
    
    void work() override;
    virtual void receiveData() = 0;
    void deleteSuperfluousPoints(const uint index);
    
    virtual ~UtilizationWorker();
protected:
    long lastTime = 0;
};

class GPUUtilizationWorker : public UtilizationWorker {
public:
    void receiveData() override;
};

class MemoryUtilizationWorker : public UtilizationWorker {
public:
    MemoryData *memoryData;

    MemoryUtilizationWorker();
    ~MemoryUtilizationWorker();

    void receiveData() override;
};

class UtilizationWidget : public QWidget {
    Q_OBJECT
public:
    std::vector<QRect> statusObjectsAreas;
    UtilizationWorker *worker;

    ~UtilizationWidget();

    void paintEvent(QPaintEvent*) override;

public slots:
    void onDataUpdated();
};

class GPUUtilization : public UtilizationWidget {
public:
    GPUUtilization();

    void mouseMoveEvent(QMouseEvent *event) override;
};

class MemoryUtilization : public UtilizationWidget {
public:
    MemoryUtilization();

    void mouseMoveEvent(QMouseEvent *event) override;
};

void drawGrid(QWidget *widget, QPainter *p);
void drawGraph(UtilizationWorker *worker, QPainter *p);
void drawStatusObjects(std::vector<QRect> &statusObjectsAreas, UtilizationData *udata, QPainter *p);

#endif
