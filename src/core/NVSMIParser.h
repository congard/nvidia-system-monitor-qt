#ifndef QNVSM_NVSMIPARSER_H
#define QNVSM_NVSMIPARSER_H

#include <QRegularExpression>
#include <QString>
#include <QVector>

#include "processes/ProcessInfo.h"
#include "utilization/memory/MemoryData.h"
#include "power/PowerInfo.h"

class NVSMIParser {
public:
    constexpr static int INT_NONE = INT32_MIN;

public:
    static void init();

    static QVector<ProcessInfo> getProcesses();
    static QVarLengthArray<int> getGPUUtilization();
    static QVarLengthArray<MemoryData> getMemoryUtilization();
    static QVarLengthArray<PowerInfo> getPowerInfo();
    static QVarLengthArray<QString> getGPUNames();
    static int getGPUCount();

private:
    static QRegularExpression processListRegex;
};

#endif //QNVSM_NVSMIPARSER_H
