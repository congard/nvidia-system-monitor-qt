#ifndef QNVSM_GPUUTILIZATIONCONTAINER_H
#define QNVSM_GPUUTILIZATIONCONTAINER_H

#include <QLabel>
#include <vector>
#include <array>

#include "utilization/UtilizationContainer.h"

class GPUUtilizationContainer: public UtilizationContainer {
    Q_OBJECT

public:
    GPUUtilizationContainer();

public slots:
    void onDataUpdated();

private:
    std::vector<std::array<QLabel*, 2>> infoLabels;
};


#endif //QNVSM_GPUUTILIZATIONCONTAINER_H
