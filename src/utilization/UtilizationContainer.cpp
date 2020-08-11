#include "UtilizationContainer.h"

#include <QHBoxLayout>
#include <QLabel>

#include "core/Utils.h"
#include "Settings.h"
#include "CircleWidget.h"

void UtilizationContainer::build(const QString &name) {
    auto title = new QLabel();
    title->setText("<b><big>" + name + "</big></b>");
    addWidget(title);

    auto headerLayout = new QHBoxLayout();
    auto footerLayout = new QHBoxLayout();

    auto graphStep = new QLabel();
    graphStep->setText((Utils::toString(Settings::UpdateDelay / 1000.0f) + " sec step").c_str());
    headerLayout->addWidget(graphStep, 0, Qt::AlignLeft);

    auto fullUtilization = new QLabel();
    fullUtilization->setText("100%");
    headerLayout->addWidget(fullUtilization, 0, Qt::AlignRight);

    auto graphTime = new QLabel();
    graphTime->setText((Utils::toString(Settings::GraphLength / 1000.0f) + " sec").c_str());
    footerLayout->addWidget(graphTime, 0, Qt::AlignLeft);

    auto noUtilization = new QLabel();
    noUtilization->setText("0%");
    footerLayout->addWidget(noUtilization, 0, Qt::AlignRight);

    setAlignment(Qt::AlignVCenter);

    addLayout(headerLayout);
    addWidget(utilizationWidget);
    addLayout(footerLayout);
}

QWidget* UtilizationContainer::getWidget() {
    auto widget = new QWidget();
    widget->setLayout(this);

    return widget;
}

UtilizationWorker* UtilizationContainer::getWorker() {
    return utilizationWidget->worker;
}

void UtilizationContainer::addInfoTitleLayout(int gpuIndex) {
    auto gpuName = new QLabel();
    gpuName->setText("<b>GPU " + QString::number(gpuIndex + 1) + "</b>");

    int fontHeight = gpuName->fontMetrics().height();

    auto circle = new CircleWidget(Settings::GPUColors[gpuIndex]);
    circle->setMinimumSize(fontHeight, fontHeight);

    auto titleLayout = new QHBoxLayout();
    titleLayout->setAlignment(Qt::AlignLeft);
    titleLayout->setContentsMargins(0, fontHeight / 2, 0, 0);

    titleLayout->addWidget(gpuName);
    titleLayout->addWidget(circle);

    addLayout(titleLayout);
}
