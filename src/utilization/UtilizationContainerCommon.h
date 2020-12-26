#ifndef QNVSM_UTILIZATIONCONTAINERCOMMON_H
#define QNVSM_UTILIZATIONCONTAINERCOMMON_H

#include <QLabel>

inline QString getInfoLabelName(uint gpuIndex, uint index) {
    return "il_" + QString::number(gpuIndex) + "_" + QString::number(index);
}

inline QLabel* getInfoLabel(uint gpuIndex, uint index) {
    auto label = new QLabel();
    label->setObjectName(getInfoLabelName(gpuIndex, index));

    return label;
}

#endif //QNVSM_UTILIZATIONCONTAINERCOMMON_H
