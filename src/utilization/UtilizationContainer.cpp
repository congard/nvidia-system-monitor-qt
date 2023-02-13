#include "UtilizationContainer.h"

#include <QLabel>

#include "core/SettingsManager.h"
#include "core/InfoProvider.h"

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

    m_descLayout = new FlowGridLayout(0, 12, 6);
    layout->addLayout(m_descLayout);

    setLayout(layout);

    connect(InfoProvider::getWorker(), &InfoProvider::Worker::dataUpdated, this, &UtilizationContainer::dataUpdated);
}

void UtilizationContainer::updateLegend() {
    findChild<QLabel*>(graphStepName)->setText(QString::number(SettingsManager::getUpdateDelay() / 1000.0f) + " sec step");
    findChild<QLabel*>(graphTimeName)->setText(QString::number(SettingsManager::getGraphLength() / 1000.0f) + " sec");
    emit onLegendUpdate();
}

void UtilizationContainer::dataUpdated() {
    emit onDataUpdated();
}

UtilizationWidget* UtilizationContainer::getUtilizationWidget() const {
    return utilizationWidget;
}

FlowGridLayout* UtilizationContainer::getDescLayout() {
    return m_descLayout;
}
