#ifndef QNVSM_SELECTCOLORBUTTON_H
#define QNVSM_SELECTCOLORBUTTON_H

#include <QPushButton>

class SelectColorButton: public QPushButton {
    Q_OBJECT

public:
    explicit SelectColorButton(const QColor &color);

    void setColor(const QColor &color);
    const QColor& getColor() const;

private slots:
    void changeColor();

private:
    QColor m_color;
};

#endif //QNVSM_SELECTCOLORBUTTON_H
