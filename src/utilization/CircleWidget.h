#ifndef QNVSM_CIRCLEWIDGET_H
#define QNVSM_CIRCLEWIDGET_H

#include <QWidget>
#include <QPen>
#include <QBrush>

class CircleWidget: public QWidget {
public:
    CircleWidget();

    void setColor(const QColor &color);

    void paintEvent(QPaintEvent*) override;

private:
    QPen pen;
    QBrush brush;
};


#endif //QNVSM_CIRCLEWIDGET_H
