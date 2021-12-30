#include "NVSMIParser.h"
#include "Utils.h"
#include "SettingsManager.h"

#include <cmath>
#include <unistd.h>

using namespace std;
using namespace Utils;

#define constant(name, data) constexpr char name[] = data

constant(GPUCountCommand, "nvidia-smi --query-gpu=count --format=csv,noheader");
constant(GPUUtilizationCommand, "nvidia-smi --query-gpu=utilization.gpu --format=csv,noheader,nounits");
constant(GPUNamesCommand, "nvidia-smi --query-gpu=name --format=csv,noheader");

// nvidia-smi process output indices
namespace NVSMIProcess {
enum {
    GPUIdx = 1,
    PID,
    Type,
    Sm,
    Mem,
    Enc,
    Dec,
    FbMem,
    Name
};

constant(Command, "nvidia-smi pmon -c 1 -s um");
}

namespace NVSMIMemoryUtilization {
enum {
    IOUtilization,
    Total,
    Free,
    Used
};

constant(Command,
         "nvidia-smi --query-gpu=utilization.memory,memory.total,memory.free,memory.used "
         "--format=csv,noheader,nounits");
}

QRegularExpression NVSMIParser::processListRegex;

inline QString repeatString(const QString &str, int times) {
    QString result;

    for (int i = 0; i < times; i++)
        result += str;

    return result;
}

/* Process list example:

# gpu        pid  type    sm   mem   enc   dec    fb   command
# Idx          #   C/G     %     %     %     %    MB   name
    0       4673     G     -     -     -     -     0   java
    0       4765     G     0     0     -     -     4   Xorg

 * Parse pattern:
 *  n           n  C/G   n/-   n/-   n/-   n/-   n/-   process name
 */

#define spacer R"([ \t]*)"

void NVSMIParser::init() {
    QString processListPattern =
            repeatString(spacer "([0-9]+)", 2) + // gpu id, pid
            spacer "([CG])" + // type
            repeatString(spacer R"(([0-9\-]+))", 5) + // sm, mem, enc, dec, fb mem
            spacer R"(([^ \t\n]+))"; // process name

    processListRegex.setPattern(processListPattern);
}

QVector<ProcessInfo> NVSMIParser::getProcesses() {
    QVector<ProcessInfo> info;

    QRegularExpressionMatchIterator i = processListRegex.globalMatch(exec(NVSMIProcess::Command));

    while (i.hasNext()) {
        QRegularExpressionMatch match = i.next();

        info.append({
             match.captured(NVSMIProcess::Name), match.captured(NVSMIProcess::Type),
             match.captured(NVSMIProcess::GPUIdx), match.captured(NVSMIProcess::PID),
             match.captured(NVSMIProcess::Sm), match.captured(NVSMIProcess::Mem),
             match.captured(NVSMIProcess::Enc), match.captured(NVSMIProcess::Dec),
             match.captured(NVSMIProcess::FbMem)
        });
    }

    return info;
}

QVarLengthArray<int> NVSMIParser::getGPUUtilization() {
    QVarLengthArray<int> result(SettingsManager::getGPUCount());

    auto utilizationStr = exec(GPUUtilizationCommand).split("\n");

    for (int i = 0; i < result.size(); i++)
        result[i] = utilizationStr[i].toInt();

    return result;
}

QVarLengthArray<MemoryData> NVSMIParser::getMemoryUtilization() {
    QVarLengthArray<MemoryData> result(SettingsManager::getGPUCount());

    auto allGPUsStr = exec(NVSMIMemoryUtilization::Command).split("\n");

    for (int i = 0; i < result.size(); i++) {
        auto data = allGPUsStr[i].split(", ");

        result[i] = {
                data[NVSMIMemoryUtilization::IOUtilization].toInt(),
                data[NVSMIMemoryUtilization::Total].toInt(),
                data[NVSMIMemoryUtilization::Free].toInt(),
                data[NVSMIMemoryUtilization::Used].toInt()
        };
    }

    return result;
}

QVarLengthArray<PowerInfo> NVSMIParser::getPowerInfo() {
    QVarLengthArray<PowerInfo> result(SettingsManager::getGPUCount());

    // 123, 456, 78.7
    auto cmd = "nvidia-smi --query-gpu=temperature.gpu,temperature.memory,power.draw --format=csv,noheader,nounits";
    int attempt = 0;
    QString data = exec(cmd);

    // fix "[Unknown Error]" for power output
    while (data.contains('[') && attempt < 3) {
        usleep(50000);
        data = exec(cmd);
        ++attempt;
    }

    auto lines = data.split("\n");

    for (int i = 0; i < result.size(); ++i) {
        auto line = lines[i].split(", ");

        bool ok;

        int val = QString(line[0]).toInt(&ok);
        result[i].gpuTemp = ok ? val : NVSMIParser::INT_NONE;

        val = QString(line[1]).toInt(&ok);
        result[i].memTemp = ok ? val : NVSMIParser::INT_NONE;

        float power = QString(line[2]).toFloat(&ok);
        result[i].power = ok ? power : NAN;
    }

    return result;
}

QVarLengthArray<QString> NVSMIParser::getGPUNames() {
    QVarLengthArray<QString> result(SettingsManager::getGPUCount());

    auto names = exec(GPUNamesCommand).split("\n");

    for (int i = 0; i < result.size(); i++)
        result[i] = names[i];

    return result;
}

int NVSMIParser::getGPUCount() {
    return exec(GPUCountCommand).split("\n")[0].toInt();
}
