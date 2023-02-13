#include "CircleWidget.h"

#include <QPainter>
#include <QPaintEvent>

CircleWidget::CircleWidget() {
    pen.setWidth(2);

    brush.setStyle(Qt::BrushStyle::SolidPattern);

    setSizePolicy(QSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum));
}

void CircleWidget::setColor(const QColor &color) {
    pen.setColor(color);

    QColor brushColor = color;
    brushColor.setAlpha(64);

    brush.setColor(brushColor);
}

void CircleWidget::paintEvent(QPaintEvent *event) {
    int margin = width() / 6;

    // pen.width() / 2 because when painting a shape with an outline, we have to factor in the width of the pen
    int offset = pen.width() / 2 + margin;

    if (offset >= width() / 2)
        offset -= margin;

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setPen(pen);
    painter.setBrush(brush);
    painter.drawEllipse(rect().adjusted(offset, offset, -offset, -offset));
}
