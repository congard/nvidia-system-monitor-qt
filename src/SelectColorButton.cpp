#include "SelectColorButton.h"

#include <QColorDialog>

SelectColorButton::SelectColorButton(const QColor &color) {
    setColor(color);

    connect(this, SIGNAL(clicked()), this, SLOT(changeColor()));
}

void SelectColorButton::changeColor() {
    QColorDialog dialog(m_color, parentWidget());

    if (dialog.exec() == QDialog::Accepted) {
        if (QColor color = dialog.currentColor(); color != m_color) {
            setColor(color);
        }
    }
}

void SelectColorButton::setColor(const QColor &color) {
    m_color = color;

    setStyleSheet("background-color: " + m_color.name());
}

const QColor& SelectColorButton::getColor() const {
    return m_color;
}
