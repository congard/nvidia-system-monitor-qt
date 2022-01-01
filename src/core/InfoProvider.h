#ifndef QNVSM_INFOPROVIDER_H
#define QNVSM_INFOPROVIDER_H

#include <QVarLengthArray>

#include "Worker.h"
#include "GPUInfo.h"

/**
 * This class provides current gpu statistics
 */
class InfoProvider {
public:
    class Worker: public ::Worker {
    public:
        inline void work() override {
            InfoProvider::updateData();
        }
    };

public:
    static void init();
    static void destroy();
    static void updateData();

    static bool isGPUTempSupported(int id);
    static bool isMemTempSupported(int id);
    static bool isGPUFreqSupported(int id);
    static bool isMemFreqSupported(int id);
    static bool isPowerSupported(int id);

    static int getGPUTemp(int id);
    static int getMemTemp(int id);
    static int getGPUFreq(int id);
    static int getMemFreq(int id);
    static float getPower(int id);

    static Worker* getWorker();

private:
    static QVarLengthArray<GPUInfo> m_info;
    static Worker *m_worker;
};

#endif //QNVSM_INFOPROVIDER_H
