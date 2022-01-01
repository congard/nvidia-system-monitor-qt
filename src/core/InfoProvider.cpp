#include "InfoProvider.h"
#include "core/NVSMIParser.h"

#include <cmath>

QVarLengthArray<GPUInfo> InfoProvider::m_info;
InfoProvider::Worker* InfoProvider::m_worker;

void InfoProvider::init() {
    m_info = NVSMIParser::getPowerInfo();
    m_worker = new Worker();
}

void InfoProvider::destroy() {
    delete m_worker;
}

void InfoProvider::updateData() {
    m_info = NVSMIParser::getPowerInfo();
}

bool InfoProvider::isGPUTempSupported(int id) {
    return m_info[id].gpuTemp != NVSMIParser::INT_NONE;
}

bool InfoProvider::isMemTempSupported(int id) {
    return m_info[id].memTemp != NVSMIParser::INT_NONE;
}

bool InfoProvider::isGPUFreqSupported(int id) {
    return m_info[id].gpuFreq != NVSMIParser::INT_NONE;
}

bool InfoProvider::isMemFreqSupported(int id) {
    return m_info[id].memFreq != NVSMIParser::INT_NONE;
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

int InfoProvider::getMemFreq(int id) {
    return m_info[id].memFreq;
}

float InfoProvider::getPower(int id) {
    return m_info[id].power;
}

InfoProvider::Worker* InfoProvider::getWorker() {
    return m_worker;
}
