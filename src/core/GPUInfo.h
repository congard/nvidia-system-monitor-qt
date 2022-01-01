#ifndef QNVSM_GPUINFO_H
#define QNVSM_GPUINFO_H

struct GPUInfo {
    int gpuFreq {0};
    int memFreq {0};
    int gpuTemp {0};
    int memTemp {0};
    float power {0};
};

#endif //QNVSM_GPUINFO_H
