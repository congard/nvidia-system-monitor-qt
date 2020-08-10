#ifndef QNVSM_MEMORYUTILIZATIONCONTAINER_H
#define QNVSM_MEMORYUTILIZATIONCONTAINER_H

#include <QLabel>
#include <vector>
#include <array>

#include "utilization/UtilizationContainer.h"

class MemoryUtilizationContainer: public UtilizationContainer {
    Q_OBJECT

public:
    MemoryUtilizationContainer();

public slots:
    void onDataUpdated();

private:
    std::vector<std::array<QLabel*, 4>> infoLabels;
};

#endif //QNVSM_MEMORYUTILIZATIONCONTAINER_H
