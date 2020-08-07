#ifndef QNVSM_UTILIZATIONDATA_H
#define QNVSM_UTILIZATIONDATA_H

struct UtilizationData {
    int level = 0; // current graph point in percents
    int avgLevel = 0, minLevel = 0, maxLevel = 0;
};

#endif //QNVSM_UTILIZATIONDATA_H
