#include "UtilizationWidget.h"

#include <QApplication>
#include <QPainterPath>

#include "core/SettingsManager.h"
#include "core/InfoProvider.h"

UtilizationWidget::UtilizationWidget() {
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
}

UtilizationWidget::~UtilizationWidget() {
    delete worker;
}

void UtilizationWidget::paintEvent(QPaintEvent *) {
    painter.begin(this);
    painter.setRenderHint(QPainter::Antialiasing);

    drawGrid();

    {
        QMutexLocker locker(&worker->mutex);
        drawGraph();
    }

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

        for (size_t i = 1; i < worker->gpoints[g].size(); i++) {
            auto graphHeight = static_cast<float>(size().height());
            auto graphWidth = static_cast<float>(size().width());

            int x1 = static_cast<int>(worker->gpoints[g][i - 1].x * graphWidth);
            int y1 = static_cast<int>(graphHeight - graphHeight / 100.0f * (float) worker->gpoints[g][i - 1].y);
            int x2 = static_cast<int>(worker->gpoints[g][i].x * graphWidth);
            int y2 = static_cast<int>(graphHeight - graphHeight / 100.0f * (float) worker->gpoints[g][i].y);

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

