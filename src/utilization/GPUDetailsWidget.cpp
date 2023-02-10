#include "GPUDetailsWidget.h"
#include "core/InfoProvider.h"
#include "core/SettingsManager.h"
#include "CircleWidget.h"
#include "UtilizationContainer.h"

#include <QLabel>
#include <QToolTip>
#include <QEvent>
#include <QHelpEvent>

GPUDetailsWidget::GPUDetailsWidget(UtilizationContainer *container, int gpuId) {
    auto layout = new QVBoxLayout(this);

    auto gpuName = new QLabel();
    gpuName->setText("<b>" + InfoProvider::getGPUName(gpuId) + "</b>");

    int fontHeight = gpuName->fontMetrics().height();

    auto circle = new CircleWidget();
    circle->setMinimumSize(fontHeight, fontHeight);

    auto titleLayout = new QHBoxLayout();
    titleLayout->setAlignment(Qt::AlignLeft);
    titleLayout->setContentsMargins(0, fontHeight / 2, 0, 0);

    titleLayout->addWidget(gpuName);
    titleLayout->addWidget(circle);

    connect(container, &UtilizationContainer::onLegendUpdate, this, [=]() {
        circle->setColor(SettingsManager::getGPUColor(gpuId));
    });

    layout->addLayout(titleLayout);

    setLayout(layout);
    setMouseTracking(true);
}

QVBoxLayout* GPUDetailsWidget::getLayout() const {
    return dynamic_cast<QVBoxLayout*>(layout());
}

bool GPUDetailsWidget::event(QEvent *event) {
    if (event->type() == QEvent::ToolTip) {
        auto helpEvent = static_cast<QHelpEvent*>(event);

        if (auto child = childAt(helpEvent->pos()); child && !child->toolTip().isEmpty()) {
            QToolTip::showText(helpEvent->globalPos(), child->toolTip());
            return true;
        }

        QToolTip::hideText();
        event->ignore();

        return true;
    }

    return QWidget::event(event);
}
