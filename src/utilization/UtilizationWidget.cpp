#include "UtilizationWidget.h"

#include <QApplication>
#include <QPainterPath>

#include "core/SettingsManager.h"
#include "core/InfoProvider.h"
#include "core/Utils.h"
#include "core/PCHIPInterpolator.h"
#include "UtilizationContainer.h"

using namespace Utils;

UtilizationWidget::UtilizationWidget(UtilizationContainer *container) {
    graphPoints.resize(InfoProvider::getGPUCount());
    utilizationData.resize(InfoProvider::getGPUCount());

    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    setMinimumHeight(96);

    connect(container, &UtilizationContainer::onDataUpdated, this, &UtilizationWidget::onDataUpdated);
}

UtilizationWidget::~UtilizationWidget() = default;

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

        points.emplace_back(1.0f, (float) data.level / 100.0f);

        // deleteSuperfluousPoints(g)
        if (points.size() > 2 && points[0].x < 0 && points[1].x <= 0) {
            points.erase(points.begin());
        }

        // calculate average, min, max
        data.avgLevel = data.maxLevel = 0;
        data.minLevel = 100;

        for (auto &point : points) {
            auto level = static_cast<int>(point.y * 100.0f);
            data.avgLevel += level;

            if (data.maxLevel < level) {
                data.maxLevel = level;
            }

            if (data.minLevel > level) {
                data.minLevel = level;
            }
        }

        data.avgLevel /= static_cast<int>(points.size());
    }

    lastTime = getTime();

    update();

    emit onUpdated();
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

    for (int i = 0; i <= 4; ++i) {
        float k = (float) i / 4.0f;
        painter.drawLine(graphWidth * k, 0, graphWidth * k, graphHeight); // vertical
        painter.drawLine(0, graphHeight * k, graphWidth, graphHeight * k); // horizontal
    }
}

void UtilizationWidget::drawGraph() {
    auto graphWidth = static_cast<float>(size().width());
    auto graphHeight = static_cast<float>(size().height());

    QColor color;
    QPen pen;
    pen.setWidth(2);

    auto xPos = [graphWidth](float x) { return x * graphWidth; };
    auto yPos = [graphHeight](float y) { return graphHeight - graphHeight * y; };

    auto drawGraph = [&](const std::vector<PointF> &points) {
        QPainterPath filling;

        auto pointPos = [&](PointF point) -> QPointF {
            return {xPos(point.x), yPos(point.y)};
        };

        filling.moveTo(pointPos(points[0]));

        for (size_t i = 1; i < points.size(); i++)
            filling.lineTo(pointPos(points[i]));

        painter.drawPath(filling);

        filling.lineTo(pointPos({1.0f, 0.0f}));
        filling.lineTo(pointPos({points[0].x, 0.0f}));
        filling.lineTo(pointPos({points[0].x, points[0].y}));

        color.setAlpha(64);
        painter.fillPath(filling, QBrush(color));
    };

    for (int g = 0; g < InfoProvider::getGPUCount(); g++) {
        color = SettingsManager::getGPUColor(g);
        pen.setColor(color);
        painter.setPen(pen);

        auto &gPoints = graphPoints[g];

        if (SettingsManager::isSmoothGraph()) {
            int count = std::max(
                static_cast<int>(gPoints.size()),
                static_cast<int>(xPos(gPoints.back().x - gPoints.front().x) / 3)
            );

            if (count > gPoints.size()) {
                auto points = PCHIPInterpolator::getInterpolatedPoints(gPoints, {graphWidth, graphHeight}, count);
                drawGraph(points);
            } else {
                drawGraph(gPoints);
            }
        } else {
            drawGraph(gPoints);
        }
    }
}
