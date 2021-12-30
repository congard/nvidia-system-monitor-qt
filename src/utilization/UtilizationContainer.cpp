#include "UtilizationContainer.h"

#include <QHBoxLayout>
#include <QLabel>
#include <QEvent>
#include <QHelpEvent>
#include <QToolTip>

#include "core/Utils.h"
#include "core/SettingsManager.h"

#include "CircleWidget.h"

inline QString getGPUCircleWidgetName(uint index) {
    return "gpuCircle_" + QString::number(index);
}

inline constexpr auto graphStepName = "graphStep";
inline constexpr auto graphTimeName = "graphTime";

void UtilizationContainer::build(const QString &name) {
    auto layout = new QVBoxLayout();

    auto title = new QLabel();
    title->setText("<b><big>" + name + "</big></b>");
    layout->addWidget(title);

    auto headerLayout = new QHBoxLayout();
    auto footerLayout = new QHBoxLayout();

    auto graphStep = new QLabel();
    graphStep->setObjectName(graphStepName);
    headerLayout->addWidget(graphStep, 0, Qt::AlignLeft);

    auto fullUtilization = new QLabel();
    fullUtilization->setText("100%");
    headerLayout->addWidget(fullUtilization, 0, Qt::AlignRight);

    auto graphTime = new QLabel();
    graphTime->setObjectName(graphTimeName);
    footerLayout->addWidget(graphTime, 0, Qt::AlignLeft);

    auto noUtilization = new QLabel();
    noUtilization->setText("0%");
    footerLayout->addWidget(noUtilization, 0, Qt::AlignRight);

    layout->setAlignment(Qt::AlignVCenter);

    layout->addLayout(headerLayout);
    layout->addWidget(utilizationWidget);
    layout->addLayout(footerLayout);

    setLayout(layout);
}

void UtilizationContainer::updateData() {
    findChild<QLabel *>(graphStepName)->setText(QString::number(SettingsManager::getUpdateDelay() / 1000.0f) + " sec step");
    findChild<QLabel *>(graphTimeName)->setText(QString::number(SettingsManager::getGraphLength() / 1000.0f) + " sec");

    for (uint i = 0; i < SettingsManager::getGPUCount(); i++) {
        findChild<CircleWidget *>(getGPUCircleWidgetName(i))->setColor(SettingsManager::getGPUColor(i));
    }
}

UtilizationWorker* UtilizationContainer::getWorker() {
    return utilizationWidget->worker;
}

bool UtilizationContainer::event(QEvent *event) {
    if (event->type() == QEvent::ToolTip) {
        auto *helpEvent = static_cast<QHelpEvent*>(event);

        if (auto *child = childAt(helpEvent->pos()); child) {
            auto name = child->objectName();

            for (int i = 0; i < SettingsManager::getGPUCount(); i++) {
                if (name == getInfoLabelName(i, UtInfoLabelId)) {
                    QToolTip::showText(helpEvent->globalPos(), "Utilization: current (average / min / max)");
                    return true;
                }
            }
        }

        QToolTip::hideText();
        event->ignore();

        return true;
    }

    return QWidget::event(event);
}

void UtilizationContainer::addInfoTitleLayout(int gpuIndex) {
    auto gpuName = new QLabel();
    gpuName->setText("<b>" + SettingsManager::getGPUName(gpuIndex) + "</b>");

    int fontHeight = gpuName->fontMetrics().height();

    auto circle = new CircleWidget();
    circle->setMinimumSize(fontHeight, fontHeight);
    circle->setObjectName(getGPUCircleWidgetName(gpuIndex));

    auto titleLayout = new QHBoxLayout();
    titleLayout->setAlignment(Qt::AlignLeft);
    titleLayout->setContentsMargins(0, fontHeight / 2, 0, 0);

    titleLayout->addWidget(gpuName);
    titleLayout->addWidget(circle);

    getLayout()->addLayout(titleLayout);
}

QVBoxLayout* UtilizationContainer::getLayout() {
    return static_cast<QVBoxLayout *>(layout()); // static_cast because layout() is 100% QVBoxLayout
}
