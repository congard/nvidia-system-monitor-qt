#include "UtilizationWidget.h"

#include <QApplication>
#include <QPainterPath>

#include "core/SettingsManager.h"
#include "core/InfoProvider.h"
#include "core/Utils.h"

using namespace Utils;

UtilizationWidget::UtilizationWidget() {
    graphPoints.resize(InfoProvider::getGPUCount());
    utilizationData.resize(InfoProvider::getGPUCount());
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
}

UtilizationWidget::~UtilizationWidget() {

}

void UtilizationWidget::onDataUpdated() {
    if (lastTime == 0) {
        lastTime = getTime();
    }

    auto graphStep = (float) SettingsManager::getUpdateDelay() / (float) SettingsManager::getGraphLength();
    auto step = (float)(getTime() - lastTime) / (float) SettingsManager::getUpdateDelay() * graphStep;

    // g means gpu
    for (int g = 0; g < InfoProvider::getGPUCount(); g++) {
        auto &points = graphPoints[g];
        auto &data = utilizationData[g];

        for (PointF &point : points)
            point.x -= step;

        points.emplace_back(1.0f, data.level);

        // deleteSuperfluousPoints(g)
        if (points.size() > 2 && points[0].x < 0 && points[1].x <= 0) {
            points.erase(points.begin());
        }

        // calculate average, min, max
        data.avgLevel = data.maxLevel = 0;
        data.minLevel = 100;

        for (auto &point : points) {
            data.avgLevel += point.y;

            if (data.maxLevel < point.y) {
                data.maxLevel = point.y;
            }

            if (data.minLevel > point.y) {
                data.minLevel = point.y;
            }
        }

        data.avgLevel /= static_cast<int>(points.size());
    }

    lastTime = getTime();
}

void UtilizationWidget::paintEvent(QPaintEvent *) {
    painter.begin(this);
    painter.setRenderHint(QPainter::Antialiasing);

    drawGrid();
    drawGraph();

    painter.end();
}

void UtilizationWidget::drawGrid() {
    const int graphHeight = size().height();
    const int graphWidth = size().width();

    painter.setPen(QColor(100, 100, 100));

    for (float i = 0; i <= 1.0f; i += 0.25f) {
        painter.drawLine(graphWidth * i, 0, graphWidth * i, graphHeight);
        painter.drawLine(0, graphHeight * i, graphWidth, graphHeight * i);
    }
}

void UtilizationWidget::drawGraph() {
    QColor color;
    QPen pen;
    pen.setWidth(2);

    for (int g = 0; g < InfoProvider::getGPUCount(); g++) {
        color = SettingsManager::getGPUColor(g);
        pen.setColor(color);
        painter.setPen(pen);

        auto &points = graphPoints[g];

        for (size_t i = 1; i < points.size(); i++) {
            auto graphHeight = static_cast<float>(size().height());
            auto graphWidth = static_cast<float>(size().width());

            int x1 = static_cast<int>(points[i - 1].x * graphWidth);
            int y1 = static_cast<int>(graphHeight - graphHeight / 100.0f * (float) points[i - 1].y);
            int x2 = static_cast<int>(points[i].x * graphWidth);
            int y2 = static_cast<int>(graphHeight - graphHeight / 100.0f * (float) points[i].y);

            QPainterPath filling;
            filling.moveTo(x1, y1);
            filling.lineTo(x2, y2);
            filling.lineTo(x2, graphHeight);
            filling.lineTo(x1, graphHeight);
            filling.lineTo(x1, y1);
            color.setAlpha(64);
            painter.fillPath(filling, QBrush(color));

            painter.drawLine(x1, y1, x2, y2);
        }
    }
}

