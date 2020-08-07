#include "Graph.h"

#include <QApplication>
#include <QPainterPath>

#include "Settings.h"
#include "core/Utils.h"

using namespace Utils;
using namespace Settings;

#define graphMinHeight 150
int graphStartY, graphEndY, width;

// TODO: create class GraphWidget
// TODO: create namespace NVSMParser

inline int max(const int p1, const int p2) {
    return p1 > p2 ? p1 : p2;
}

namespace Graph {
void drawGrid(QWidget *widget, QPainter *p) {
    QFontMetrics fm(qApp->font());

    int x0;
    int y0 = fm.height();
    int graphHeight = max(graphMinHeight, (int)((float)widget->size().height() * 0.45f));

    width = widget->size().width() - 4;

    p->setPen(QColor(100, 100, 100));
    graphStartY = y0 * 1.25f;
    graphEndY = graphStartY + graphHeight;

    for (float i = 0; i <= 1.0f; i += 0.25f) {
        p->drawLine(width * i, graphStartY, width * i, graphStartY + graphHeight);
        p->drawLine(0, graphStartY + graphHeight * i, width, graphStartY + graphHeight * i);
    }

    p->setPen(QApplication::palette().text().color());
    p->drawText(0, y0, (toString(UpdateDelay / 1000.0f) + " sec step").c_str());
    p->drawText(0, graphEndY + y0, (toString(GraphLength / 1000.0f) + " sec").c_str());

    QString text = "100%";
    x0 = fm.horizontalAdvance(text);
    p->drawText(width - x0, y0, text);

    text = "0%";
    x0 = fm.horizontalAdvance(text);
    p->drawText(width - x0, graphEndY + y0, text);
}

void drawGraph(UtilizationWorker *worker, QPainter *p) {
    int x1, y1, x2, y2;
    QColor color;
    QPen pen;
    pen.setWidth(2);

    for (int g = 0; g < GPUCount; g++) {
        color = GPUColors[g];
        pen.setColor(color);
        p->setPen(pen);

        for (size_t i = 1; i < worker->gpoints[g].size(); i++) {
            x1 = worker->gpoints[g][i - 1].x * width;
            y1 = graphEndY - (graphEndY - graphStartY) / 100.0f * worker->gpoints[g][i - 1].y;
            x2 = worker->gpoints[g][i].x * width;
            y2 = graphEndY - (graphEndY - graphStartY) / 100.0f * worker->gpoints[g][i].y;

            QPainterPath filling;
            filling.moveTo(x1, y1);
            filling.lineTo(x2, y2);
            filling.lineTo(x2, graphEndY);
            filling.lineTo(x1, graphEndY);
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

    int blockSize = size + StatusObjectTextOffset + textWidth + StatusObjectOffset;
    int horizontalCount = (width + StatusObjectOffset) / blockSize; // (width + STATUS_OBJECT_OFFSET) because last element has offset
    QRect progress;

    for (int g = 0; g < GPUCount; g++) {
        p->setPen(GPUColors[g]);
        p->setBrush(QBrush(GPUColors[g]));

        int x = blockSize * (g % horizontalCount);
        int y = graphEndY + fm.height() + (size + StatusObjectOffset) * (g / horizontalCount) + GraphOffset;
        int spanAngle = -udata[g].level / 100.0f * 360;

        progress = QRect(x, y, size, size);

        QPainterPath progressPath;
        progressPath.moveTo(x + size / 2, y + size / 2);
        progressPath.arcTo(progress, 90, spanAngle);
        p->drawPath(progressPath);

        p->setPen(QApplication::palette().text().color());
        p->setBrush(QBrush());
        p->drawEllipse(x, y, size, size);

        p->drawText(x + size + StatusObjectTextOffset, y + size / 2 + fm.xHeight() / 2, (std::to_string(udata[g].level) + "%").c_str());

        statusObjectsAreas.emplace_back(x, y, blockSize, size);
    }
}
}
