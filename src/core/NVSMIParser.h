#ifndef QNVSM_NVSMIPARSER_H
#define QNVSM_NVSMIPARSER_H

#include <QRegularExpression>
#include <QString>

#include "processes/ProcessInfo.h"
#include "utilization/memory/MemoryData.h"

class NVSMIParser {
public:
    static void init();

    static QVector<ProcessInfo> getProcesses();
    static QVarLengthArray<int> getGPUUtilization();
    static QVarLengthArray<MemoryData> getMemoryUtilization();
    static int getGPUCount();

private:
    static QRegularExpression processListRegex;
};

#endif //QNVSM_NVSMIPARSER_H
