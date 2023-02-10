#ifndef QNVSM_INFOPROVIDER_H
#define QNVSM_INFOPROVIDER_H

#include <QRegularExpression>
#include <QVarLengthArray>
#include <QVector>

#include "processes/ProcessInfo.h"
#include "Worker.h"

/**
 * This class provides actual gpu information
 */
class InfoProvider {
public:
    class Worker: public ::Worker {
    public:
        inline void work() override {
            QMutexLocker locker(&mutex);
            InfoProvider::updateData();
            dataUpdated();
        }
    };

    struct GPUInfo {
        int gpuFreq {0};
        int gpuFreqMax {0};
        int memFreq {0};
        int memFreqMax {0};
        int gpuTemp {0};
        int memTemp {0};
        float power {0};

        int gpuUtil {0};
        int memUtil {0};
        int memTotal {0};
        int memFree {0};
        int memUsed {0};
    };

    constexpr static int INT_NONE = INT32_MIN;

public:
    static void init();
    static void destroy();
    static void updateData();

    static const QVector<ProcessInfo>& getProcesses();

    static bool isGPUTempSupported(int id);
    static bool isMemTempSupported(int id);
    static bool isGPUFreqSupported(int id);
    static bool isMemFreqSupported(int id);
    static bool isPowerSupported(int id);

    static int getGPUTemp(int id);
    static int getMemTemp(int id);
    static int getGPUFreq(int id);
    static int getGPUFreqMax(int id);
    static int getMemFreq(int id);
    static int getMemFreqMax(int id);
    static float getPower(int id);
    static int getGPUUtil(int id);
    static int getMemUtil(int id);
    static int getMemTotal(int id);
    static int getMemFree(int id);
    static int getMemUsed(int id);

    static int getGPUCount();
    static const QString& getGPUName(int id);

    static Worker* getWorker();

private:
    static QRegularExpression m_processListRegex;
    static QVector<ProcessInfo> m_processes;

    static QVarLengthArray<GPUInfo> m_info;
    static QVarLengthArray<QString> m_gpuNames;
    static Worker *m_worker;
    static int m_gpuCount;

    static QString m_infoQueryCmd;
    static int m_iGpuTemp;
    static int m_iMemTemp;
    static int m_iGpuFreq;
    static int m_iGpuFreqMax;
    static int m_iMemFreq;
    static int m_iMemFreqMax;
    static int m_iPower;
    static int m_iGpuUtil;
    static int m_iMemUtil;
    static int m_iMemTotal;
    static int m_iMemFree;
    static int m_iMemUsed;
};

#endif //QNVSM_INFOPROVIDER_H
