#include "InfoProvider.h"
#include "Utils.h"

#include <cmath>
#include <thread>
#include <array>

using namespace Utils;

QRegularExpression InfoProvider::m_processListRegex;
QVector<ProcessInfo> InfoProvider::m_processes;

QVarLengthArray<InfoProvider::GPUInfo> InfoProvider::m_info;
QVarLengthArray<QString> InfoProvider::m_gpuNames;
InfoProvider::Worker* InfoProvider::m_worker;
int InfoProvider::m_gpuCount {0};

QString InfoProvider::m_infoQueryCmd;
int InfoProvider::m_iGpuTemp {INT_NONE};
int InfoProvider::m_iMemTemp {INT_NONE};
int InfoProvider::m_iGpuFreq {INT_NONE};
int InfoProvider::m_iGpuFreqMax {INT_NONE};
int InfoProvider::m_iMemFreq {INT_NONE};
int InfoProvider::m_iMemFreqMax {INT_NONE};
int InfoProvider::m_iPower {INT_NONE};
int InfoProvider::m_iGpuUtil {INT_NONE};
int InfoProvider::m_iMemUtil {INT_NONE};
int InfoProvider::m_iMemTotal {INT_NONE};
int InfoProvider::m_iMemFree {INT_NONE};
int InfoProvider::m_iMemUsed {INT_NONE};

namespace {
constexpr auto gpuInfoPowerMaxAttemptCount{3};
constexpr auto gpuInfoPowerAttemptDelay{std::chrono::milliseconds{50}};

constexpr QStringView operator""_qsv(const char16_t* str, size_t len) noexcept {
    return QStringView{str, static_cast<qsizetype>(len)};
}
}

void InfoProvider::init() {
    initGPUCount();
    initProcessList();
    initGPUInfo();
}

void InfoProvider::initGPUCount() {
    m_gpuCount = exec_cmd("nvidia-smi --query-gpu=count --format=csv,noheader").split("\n")[0].toInt();
    m_gpuNames.resize(m_gpuCount);

    auto names = exec_cmd("nvidia-smi --query-gpu=name --format=csv,noheader").split("\n");

    for (int i = 0; i < m_gpuCount; i++)
        m_gpuNames[i] = std::move(names[i]);

    m_info.resize(m_gpuCount);
}

void InfoProvider::initProcessList() {
    // TODO: maybe to keep compatibility with older (and newer) drivers, firstly find indices,
    //  like in `initGPUInfo`, instead of hard-coded ones?

    /* Process list example:

        # gpu         pid   type     sm    mem    enc    dec    jpg    ofa     fb   ccpm    command
        # Idx           #    C/G      %      %      %      %      %      %     MB     MB    name
            0       2479     G      -      -      -      -      -      -      1      0    kwalletd6
            0       2481     G      -      -      -      -      -      -    624      0    Xorg

      * Parse pattern:
      *     n           n    C/G     n/-    n/-    n/-    n/-    n/-    n/-    n/-    n/-   process name
      */

    auto repeatString = [](const QString &str, int times) {
        QString result;

        for (int i = 0; i < times; i++)
            result += str;

        return result;
    };

#define spacer R"([ \t]*)"

    const QString processListPattern{
        repeatString(spacer "([0-9]+)", 2) + // gpu id, pid
        spacer R"((C(?:\+G)?|G))" + // type
        repeatString(spacer R"(([0-9\-]+))", 8) + // sm, mem, enc, dec, jpg, ofa, fb, ccpm
        spacer R"(([^ \t\n]+))" // process name
    };

    m_processListRegex.setPattern(processListPattern);
}

void InfoProvider::initGPUInfo() {
    const auto queryHelp = exec_cmd("nvidia-smi --help-query-gpu");

    struct Option {
        QStringView name;
        int &index;
    };

    constexpr std::array options{
        Option{u"temperature.gpu"_qsv, m_iGpuTemp},
        Option{u"temperature.memory"_qsv, m_iMemTemp},
        Option{u"clocks.gr"_qsv, m_iGpuFreq},
        Option{u"clocks.max.gr"_qsv, m_iGpuFreqMax},
        Option{u"clocks.mem"_qsv, m_iMemFreq},
        Option{u"clocks.max.mem"_qsv, m_iMemFreqMax},
        Option{u"power.draw"_qsv, m_iPower},
        Option{u"utilization.gpu"_qsv, m_iGpuUtil},
        Option{u"utilization.memory"_qsv, m_iMemUtil},
        Option{u"memory.total"_qsv, m_iMemTotal},
        Option{u"memory.free"_qsv, m_iMemFree},
        Option{u"memory.used"_qsv, m_iMemUsed}
    };

    int index{0};
    QString availableOptions;

    for (const auto [optName, optIndex] : options) {
        if (queryHelp.contains(optName)) {
            optIndex = index;
            availableOptions.append(optName);
            availableOptions.append(',');
            ++index;
        }
    }

    m_infoQueryCmd = QString::asprintf(
        "nvidia-smi --query-gpu=%s --format=csv,noheader,nounits", qPrintable(availableOptions));

    updateData();
    m_worker = new Worker();
}

void InfoProvider::destroy() {
    delete m_worker;
}

void InfoProvider::updateData() {
    updateProcessList();
    updateGPUInfo();
}

void InfoProvider::updateProcessList() {
    // nvidia-smi process output indices
    enum {
        GPUIdx = 1,
        PID,
        Type,
        Sm,
        Mem,
        Enc,
        Dec,
        Jpg,
        Ofa,
        FbMem,
        Ccpm,
        Name
    };

    m_processes = {};

    QRegularExpressionMatchIterator procIt{
        m_processListRegex.globalMatch(exec_cmd("nvidia-smi pmon -c 1 -s um"))
    };

    while (procIt.hasNext()) {
        QRegularExpressionMatch match = procIt.next();

        m_processes.append({
            .name = match.captured(Name),
            .type = match.captured(Type),
            .gpuIdx = match.captured(GPUIdx),
            .pid = match.captured(PID),
            .sm = match.captured(Sm),
            .mem = match.captured(Mem),
            .enc = match.captured(Enc),
            .dec = match.captured(Dec),
            .fbmem = match.captured(FbMem)
        });
    }
}

void InfoProvider::updateGPUInfo() {
    int attempt{0};
    QString data{exec_cmd_s(m_infoQueryCmd)};

    // fix "[Unknown Error]" for power output
    while (data.contains('[') && attempt < gpuInfoPowerMaxAttemptCount) {
        std::this_thread::sleep_for(gpuInfoPowerAttemptDelay);
        data = exec_cmd_s(m_infoQueryCmd);
        ++attempt;
    }

    const auto lines = data.split("\n");

    for (int i = 0; i < m_info.size(); ++i) {
        const auto line = lines[i].split(", ");

        auto parseIndex = [&]<typename T>(int index, T &writeTo) {
            if (index != INT_NONE) {
                bool ok;

                if constexpr (std::is_same_v<T, int>) {
                    auto val = QString(line[index]).toInt(&ok);
                    writeTo = ok ? val : INT_NONE;
                } else if constexpr(std::is_same_v<T, float>) {
                    auto val = QString(line[index]).toFloat(&ok);
                    writeTo = ok ? val : NAN;
                }
            }
        };

        parseIndex(m_iGpuTemp, m_info[i].gpuTemp);
        parseIndex(m_iMemTemp, m_info[i].memTemp);
        parseIndex(m_iGpuFreq, m_info[i].gpuFreq);
        parseIndex(m_iGpuFreqMax, m_info[i].gpuFreqMax);
        parseIndex(m_iMemFreq, m_info[i].memFreq);
        parseIndex(m_iMemFreqMax, m_info[i].memFreqMax);
        parseIndex(m_iPower, m_info[i].power);
        parseIndex(m_iGpuUtil, m_info[i].gpuUtil);
        parseIndex(m_iMemUtil, m_info[i].memUtil);
        parseIndex(m_iMemTotal, m_info[i].memTotal);
        parseIndex(m_iMemFree, m_info[i].memFree);
        parseIndex(m_iMemUsed, m_info[i].memUsed);
    }
}

const QVector<ProcessInfo>& InfoProvider::getProcesses() {
    return m_processes;
}

bool InfoProvider::isGPUTempSupported(int id) {
    return m_info[id].gpuTemp != INT_NONE;
}

bool InfoProvider::isMemTempSupported(int id) {
    return m_info[id].memTemp != INT_NONE;
}

bool InfoProvider::isGPUFreqSupported(int id) {
    return m_info[id].gpuFreq != INT_NONE;
}

bool InfoProvider::isMemFreqSupported(int id) {
    return m_info[id].memFreq != INT_NONE;
}

bool InfoProvider::isPowerSupported(int id) {
    return !std::isnan(m_info[id].power);
}

int InfoProvider::getGPUTemp(int id) {
    return m_info[id].gpuTemp;
}

int InfoProvider::getMemTemp(int id) {
    return m_info[id].memTemp;
}

int InfoProvider::getGPUFreq(int id) {
    return m_info[id].gpuFreq;
}

int InfoProvider::getGPUFreqMax(int id) {
    return m_info[id].gpuFreqMax;
}

int InfoProvider::getMemFreq(int id) {
    return m_info[id].memFreq;
}

int InfoProvider::getMemFreqMax(int id) {
    return m_info[id].memFreqMax;
}

float InfoProvider::getPower(int id) {
    return m_info[id].power;
}

int InfoProvider::getGPUUtil(int id) {
    return m_info[id].gpuUtil;
}

int InfoProvider::getMemUtil(int id) {
    return m_info[id].memUtil;
}

int InfoProvider::getMemTotal(int id) {
    return m_info[id].memTotal;
}

int InfoProvider::getMemFree(int id) {
    return m_info[id].memFree;
}

int InfoProvider::getMemUsed(int id) {
    return m_info[id].memUsed;
}

int InfoProvider::getGPUCount() {
    return m_gpuCount;
}

const QString& InfoProvider::getGPUName(int id) {
    return m_gpuNames[id];
}

InfoProvider::Worker* InfoProvider::getWorker() {
    return m_worker;
}
