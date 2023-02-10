#ifndef QNVSM_GPUMEMDETAILSWIDGET_H
#define QNVSM_GPUMEMDETAILSWIDGET_H

#include "../GPUDetailsWidget.h"

class GPUMemDetailsWidget: public GPUDetailsWidget {
public:
    GPUMemDetailsWidget(UtilizationContainer *container, int gpuId);
};


#endif //QNVSM_GPUMEMDETAILSWIDGET_H
