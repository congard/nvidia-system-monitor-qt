#ifndef QNVSM_GPUUTDETAILSWIDGET_H
#define QNVSM_GPUUTDETAILSWIDGET_H

#include "../GPUDetailsWidget.h"

class GPUUtDetailsWidget: public GPUDetailsWidget {
public:
    GPUUtDetailsWidget(UtilizationContainer *container, int gpuId);
};

#endif //QNVSM_GPUUTDETAILSWIDGET_H
