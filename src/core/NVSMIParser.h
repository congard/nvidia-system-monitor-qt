#ifndef QNVSM_NVSMIPARSER_H
#define QNVSM_NVSMIPARSER_H

#include <QRegularExpression>
#include <QString>
#include <vector>

#include "processes/ProcessInfo.h"

class NVSMIParser {
public:
    static void init();

    static std::vector<ProcessInfo> getProcesses();
    static int getGPUCount();

private:
    static QRegularExpression processListRegex;
};

#endif //QNVSM_NVSMIPARSER_H
