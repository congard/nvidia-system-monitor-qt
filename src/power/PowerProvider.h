#ifndef QNVSM_POWERPROVIDER_H
#define QNVSM_POWERPROVIDER_H

#include <QVarLengthArray>

#include "PowerWorker.h"
#include "PowerInfo.h"

/**
 * This class provides power & temperature information
 */
class PowerProvider {
public:
    static void init();
    static void destroy();
    static void updateData();

    static bool isGPUTempSupported(int id);
    static bool isMemTempSupported(int id);
    static bool isPowerSupported(int id);

    static int getGPUTemp(int id);
    static int getMemTemp(int id);
    static float getPower(int id);

    static PowerWorker* getWorker();

private:
    static QVarLengthArray<PowerInfo> m_info;
    static PowerWorker *m_worker;
};

#endif //QNVSM_POWERPROVIDER_H
