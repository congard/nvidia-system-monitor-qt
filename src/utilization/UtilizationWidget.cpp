#include "UtilizationWidget.h"

#include <QApplication>
#include <QPainterPath>

#include "core/Utils.h"
#include "Settings.h"

using namespace Settings;

QString UtilizationWidget::fullUtilizationText;
QString UtilizationWidget::noUtilizationText;
QString UtilizationWidget::graphStepSecondsText;
QString UtilizationWidget::graphSecondsText;
int UtilizationWidget::fullUtilizationTextWidth;
int UtilizationWidget::noUtilizationTextWidth;
int UtilizationWidget::textHeight;

UtilizationWidget::~UtilizationWidget() {
    delete worker;
}

void UtilizationWidget::paintEvent(QPaintEvent *) {
    painter.begin(this);
    painter.setRenderHint(QPainter::Antialiasing);
    drawGrid();
    QMutexLocker locker(&worker->mutex);
    drawGraph();
    drawStatusObjects();
    painter.end();
}

void UtilizationWidget::init() {
    fullUtilizationText = "100%";
    noUtilizationText = "0%";
    graphStepSecondsText = (Utils::toString(UpdateDelay / 1000.0f) + " sec step").c_str();
    graphSecondsText = (Utils::toString(GraphLength / 1000.0f) + " sec").c_str();

    QFontMetrics fontMetrics(qApp->font());
    textHeight = fontMetrics.height();
    fullUtilizationTextWidth = fontMetrics.horizontalAdvance(fullUtilizationText);
    noUtilizationTextWidth = fontMetrics.horizontalAdvance(noUtilizationText);
}

void UtilizationWidget::onDataUpdated() {
    update();
}

void UtilizationWidget::drawGrid() {
    // TODO: temporary solution, must be:
    //  max(graphMinHeight, (int)((float)size().height() * 0.45f));
    int graphHeight = (int)((float)size().height() * 0.45f);

    width = size().width() - 4;

    painter.setPen(QColor(100, 100, 100));
    graphStartY = textHeight * 1.25f;
    graphEndY = graphStartY + graphHeight;

    for (float i = 0; i <= 1.0f; i += 0.25f) {
        painter.drawLine(width * i, graphStartY, width * i, graphStartY + graphHeight);
        painter.drawLine(0, graphStartY + graphHeight * i, width, graphStartY + graphHeight * i);
    }

    painter.setPen(QApplication::palette().text().color());
    painter.drawText(0, textHeight, graphStepSecondsText);
    painter.drawText(0, graphEndY + textHeight, graphSecondsText);
    painter.drawText(width - fullUtilizationTextWidth, textHeight, fullUtilizationText);
    painter.drawText(width - noUtilizationTextWidth, graphEndY + textHeight, noUtilizationText);
}

void UtilizationWidget::drawGraph() {
    QColor color;
    QPen pen;
    pen.setWidth(2);

    for (int g = 0; g < GPUCount; g++) {
        color = GPUColors[g];
        pen.setColor(color);
        painter.setPen(pen);

        for (size_t i = 1; i < worker->gpoints[g].size(); i++) {
            int x1 = worker->gpoints[g][i - 1].x * width;
            int y1 = graphEndY - (graphEndY - graphStartY) / 100.0f * worker->gpoints[g][i - 1].y;
            int x2 = worker->gpoints[g][i].x * width;
            int y2 = graphEndY - (graphEndY - graphStartY) / 100.0f * worker->gpoints[g][i].y;

            QPainterPath filling;
            filling.moveTo(x1, y1);
            filling.lineTo(x2, y2);
            filling.lineTo(x2, graphEndY);
            filling.lineTo(x1, graphEndY);
            filling.lineTo(x1, y1);
            color.setAlpha(64);
            painter.fillPath(filling, QBrush(color));

            painter.drawLine(x1, y1, x2, y2);
        }
    }
}

void UtilizationWidget::drawStatusObjects() {
    statusObjectsAreas.clear();

    auto udata = worker->udata;

    QFontMetrics fm(qApp->font());
    int size = textHeight * 2; // width and height for progress arc

    int blockSize = size + StatusObjectTextOffset + fullUtilizationTextWidth + StatusObjectOffset;
    int horizontalCount = (width + StatusObjectOffset) / blockSize; // (width + STATUS_OBJECT_OFFSET) because last element has offset
    QRect progress;

    for (int g = 0; g < GPUCount; g++) {
        painter.setPen(GPUColors[g]);
        painter.setBrush(QBrush(GPUColors[g]));

        int x = blockSize * (g % horizontalCount);
        int y = graphEndY + textHeight + (size + StatusObjectOffset) * (g / horizontalCount) + GraphOffset;
        int spanAngle = -udata[g].level / 100.0f * 360;

        progress = QRect(x, y, size, size);

        QPainterPath progressPath;
        progressPath.moveTo(x + size / 2, y + size / 2);
        progressPath.arcTo(progress, 90, spanAngle);
        painter.drawPath(progressPath);

        painter.setPen(QApplication::palette().text().color());
        painter.setBrush(QBrush());
        painter.drawEllipse(x, y, size, size);

        painter.drawText(x + size + StatusObjectTextOffset, y + size / 2 + fm.xHeight() / 2, (std::to_string(udata[g].level) + "%").c_str());

        statusObjectsAreas.emplace_back(x, y, blockSize, size);
    }
}
