#include "utilization.h"
#include <iostream>
#include <QFontMetrics>
#include <QApplication>
#include <QToolTip>
#include <QMouseEvent>

#include "settings.h"
#include "utils.h"
#include "constants.h"

#define graphHeightCoef 9
int grapthStartY, grapthEndY, width;

void drawGrid(QWidget *widget, QPainter *p) {
    QFontMetrics fm(qApp->font());
    int x0, y0 = fm.height(), graphHeight = graphHeightCoef * y0;
    width = widget->size().width() - 4;
    
    p->setPen(QColor(100, 100, 100));
    grapthStartY = y0 * 1.25f;
    grapthEndY = grapthStartY + graphHeight;
    for (float i = 0; i <= 1.0f; i += 0.25) {
        p->drawLine(width * i, grapthStartY, width * i, grapthStartY + graphHeight);
        p->drawLine(0, grapthStartY + graphHeight * i, width, grapthStartY + graphHeight * i);
    }
    
    p->setPen(QApplication::palette().text().color());
    p->drawText(0, y0, (toString(UPDATE_DELAY / 1000.0f) + " sec step").c_str());
    p->drawText(0, grapthEndY + y0, (toString(GRAPH_LENGTH / 1000.0f) + " sec").c_str());
    
    QString text = "100%";
    x0 = fm.horizontalAdvance(text);
    p->drawText(width - x0, y0, text);
    
    text = "0%";
    x0 = fm.horizontalAdvance(text);
    p->drawText(width - x0, grapthEndY + y0, text);
}

void drawGraph(UtilizationWorker *worker, QPainter *p) {
    int x1, y1, x2, y2;
    QColor color;
    QPen pen;
    pen.setWidth(2);
    for (int g = 0; g < GPU_COUNT; g++) {
        color = gpuColors[g];
        pen.setColor(color);
        p->setPen(pen);
        for (size_t i = 1; i < worker->gpoints[g].size(); i++) {
            x1 = worker->gpoints[g][i - 1].x * width;
            y1 = grapthEndY - (grapthEndY - grapthStartY) / 100.0f * worker->gpoints[g][i - 1].y;
            x2 = worker->gpoints[g][i].x * width;
            y2 = grapthEndY - (grapthEndY - grapthStartY) / 100.0f * worker->gpoints[g][i].y;
                
            QPainterPath filling;
            filling.moveTo(x1, y1);
            filling.lineTo(x2, y2);
            filling.lineTo(x2, grapthEndY);
            filling.lineTo(x1, grapthEndY);
            filling.lineTo(x1, y1);
            color.setAlpha(64);
            p->fillPath(filling, QBrush(color));
                
            p->drawLine(x1, y1, x2, y2);
        }
    }
}

void drawStatusObjects(std::vector<QRect> &statusObjectsAreas, UtilizationData *udata, QPainter* p) {
    statusObjectsAreas.clear();
    QFontMetrics fm(qApp->font());
    int size = fm.height() * 2; // width and height for progress arc
    int textWidth = fm.horizontalAdvance("100%"); // because max width will be only at 100%
    
    int blockSize = size + STATUS_OBJECT_TEXT_OFFSET + textWidth + STATUS_OBJECT_OFFSET;
    int horizontalCount = (width + STATUS_OBJECT_OFFSET) / blockSize; // (width + STATUS_OBJECT_OFFSET) because last element has offset
    int spanAngle, x, y;
    QRect progress;
    
    for (int g = 0; g < GPU_COUNT; g++) {
        p->setPen(gpuColors[g]);
        p->setBrush(QBrush(gpuColors[g]));
        
        x = blockSize * (g % horizontalCount);
        y = grapthEndY + fm.height() + (size + STATUS_OBJECT_OFFSET) * (g / horizontalCount) + GRAPTH_OFFSET;
        spanAngle = -udata[g].level / 100.0f * 360;
        
        progress = QRect(x, y, size, size);
        
        QPainterPath progressPath;
        progressPath.moveTo(x + size / 2, y + size / 2);
        progressPath.arcTo(progress, 90, spanAngle);
        p->drawPath(progressPath);
        
        p->setPen(QApplication::palette().text().color());
        p->setBrush(QBrush());
        p->drawEllipse(x, y, size, size);
        
        p->drawText(x + size + STATUS_OBJECT_TEXT_OFFSET, y + size / 2 + fm.xHeight() / 2, (std::to_string(udata[g].level) + "%").c_str());
        
        statusObjectsAreas.emplace_back(x, y, blockSize, size);
    }
}

Point::Point(const float x, const int y) {
    this->x = x;
    this->y = y;
}

UtilizationWorker::UtilizationWorker() {
    gpoints = new std::vector<Point>[GPU_COUNT];
    udata = new UtilizationData[GPU_COUNT];
}

void UtilizationWorker::work() {
    mutex.lock();

    if (lastTime == 0) {
        lastTime = getTime();
        mutex.unlock();
        return;
    }
    
    receiveData();
    
    float step = (float)(getTime() - lastTime) / UPDATE_DELAY * GRAPH_STEP;
    
    // g means gpu
    for (int g = 0; g < GPU_COUNT; g++) {
        for (uint i = 0; i < gpoints[g].size(); i++)
            gpoints[g][i].x -= step;
        
        gpoints[g].emplace_back(1.0f, udata[g].level);
        deleteSuperfluousPoints(g);

        // calculate average, min, max
        udata[g].avgLevel = udata[g].maxLevel = 0;
        udata[g].minLevel = 100;
        for (uint i = 0; i < gpoints[g].size(); i++) {
            udata[g].avgLevel += gpoints[g][i].y;
            if (udata[g].maxLevel < gpoints[g][i].y)
                udata[g].maxLevel = gpoints[g][i].y;
            if (udata[g].minLevel > gpoints[g][i].y)
                udata[g].minLevel = gpoints[g][i].y;
        }
        udata[g].avgLevel /= gpoints[g].size();
    }
    
    mutex.unlock();
    dataUpdated();
    
    lastTime = getTime();
}

void UtilizationWorker::deleteSuperfluousPoints(const uint index) {
    if (gpoints[index].size() > 2 && gpoints[index][0].x < 0 && gpoints[index][1].x <= 0)
        gpoints[index].erase(gpoints[index].begin());
}

UtilizationWorker::~UtilizationWorker() {
    delete[] gpoints;
    delete[] udata;
}

void GPUUtilizationWorker::receiveData() {
    std::vector<std::string> lines = split(exec(NVSMI_CMD_GPU_UTILIZATION), "\n");
    // fake, "emulated" GPUs
//    lines[2] = std::to_string(50 + rand() % 20) + " %";
//    lines.push_back(std::to_string(30 + rand() % 10) + " %");
//    lines.push_back(std::to_string(70 + rand() % 30) + " %");
//    lines.push_back("");
    for (size_t i = 1; i < lines.size() - 1; i++)
        udata[i - 1].level = std::atoi(split(lines[i], " ")[0].c_str());
}

MemoryUtilizationWorker::MemoryUtilizationWorker() : UtilizationWorker() {
    memoryData = new MemoryData[GPU_COUNT];
}

MemoryUtilizationWorker::~MemoryUtilizationWorker() {
    delete[] memoryData;
}

void MemoryUtilizationWorker::receiveData() {
    std::vector<std::string> lines = split(exec(NVSMI_CMD_MEM_UTILIZATION), "\n"), data;
    for (size_t i = 1; i < lines.size() - 1; i++) {
        data = split(lines[i], ", ");
        udata[i - 1].level = std::atoi(split(data[0], " ")[0].c_str());
        memoryData[i - 1].total = std::atoi(split(data[1], " ")[0].c_str());
        memoryData[i - 1].free = std::atoi(split(data[2], " ")[0].c_str());
        memoryData[i - 1].used = std::atoi(split(data[3], " ")[0].c_str());
    }
}

void UtilizationWidget::paintEvent(QPaintEvent *) {
    QPainter p;
    p.begin(this);
    p.setRenderHint(QPainter::Antialiasing);
    drawGrid(this, &p);
    QMutexLocker locker(&worker->mutex);
    drawGraph(worker, &p);
    drawStatusObjects(statusObjectsAreas, worker->udata, &p);
    p.end();
}

void UtilizationWidget::onDataUpdated() {
    update();
}

UtilizationWidget::~UtilizationWidget() {
    delete worker;
}

GPUUtilization::GPUUtilization() {
    worker = new GPUUtilizationWorker;
    setMouseTracking(true);
}

#define area statusObjectsAreas[i]
void GPUUtilization::mouseMoveEvent(QMouseEvent* event) {
    for (uint i = 0; i < statusObjectsAreas.size(); i++) {
        if ((area.x() <= event->x()) && (area.x() + area.width() >= event->x()) &&
            (area.y() <= event->y()) && (area.y() + area.height() >= event->y()))
        {
            QToolTip::showText(
                event->globalPos(),
                "GPU Utilization: " + QString::number(worker->udata[i].level) +
                "\nAverage: " + QString::number(worker->udata[i].avgLevel) +
                "\nMin: " + QString::number(worker->udata[i].minLevel) +
                "\nMax: " + QString::number(worker->udata[i].maxLevel)
            );
            
            return;
        }
    }
}

MemoryUtilization::MemoryUtilization() {
    worker = new MemoryUtilizationWorker;
    setMouseTracking(true);
}

void MemoryUtilization::mouseMoveEvent(QMouseEvent* event) {
    for (uint i = 0; i < statusObjectsAreas.size(); i++) {
        if ((area.x() <= event->x()) && (area.x() + area.width() >= event->x()) &&
            (area.y() <= event->y()) && (area.y() + area.height() >= event->y()))
        {
            QToolTip::showText(
                event->globalPos(),
                "Memory Utilization: " + QString::number(worker->udata[i].level) +
                "\nAverage: " + QString::number(worker->udata[i].avgLevel) +
                "\nMin: " + QString::number(worker->udata[i].minLevel) +
                "\nMax: " + QString::number(worker->udata[i].maxLevel) +
                "\nTotal: " + QString::number(((MemoryUtilizationWorker*)worker)->memoryData[i].total) + " MiB" +
                "\nFree: " + QString::number(((MemoryUtilizationWorker*)worker)->memoryData[i].free) + " MiB" +
                "\nUsed: " + QString::number(((MemoryUtilizationWorker*)worker)->memoryData[i].used) + " MiB"
            );

            return;
        }
    }
}
