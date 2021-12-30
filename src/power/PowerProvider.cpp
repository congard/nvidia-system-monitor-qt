#include "PowerProvider.h"
#include "core/NVSMIParser.h"

#include <cmath>

QVarLengthArray<PowerInfo> PowerProvider::m_info;
PowerWorker* PowerProvider::m_worker;

void PowerProvider::init() {
    m_info = NVSMIParser::getPowerInfo();
    m_worker = new PowerWorker();
}

void PowerProvider::destroy() {
    delete m_worker;
}

void PowerProvider::updateData() {
    m_info = NVSMIParser::getPowerInfo();
}

bool PowerProvider::isGPUTempSupported(int id) {
    return m_info[id].gpuTemp != NVSMIParser::INT_NONE;
}

bool PowerProvider::isMemTempSupported(int id) {
    return m_info[id].memTemp != NVSMIParser::INT_NONE;
}

bool PowerProvider::isPowerSupported(int id) {
    return !std::isnan(m_info[id].power);
}

int PowerProvider::getGPUTemp(int id) {
    return m_info[id].gpuTemp;
}

int PowerProvider::getMemTemp(int id) {
    return m_info[id].memTemp;
}

float PowerProvider::getPower(int id) {
    return m_info[id].power;
}

PowerWorker* PowerProvider::getWorker() {
    return m_worker;
}
