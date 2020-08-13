#ifndef QNVSM_CONSTANTS_H
#define QNVSM_CONSTANTS_H

#define NVSMI_CMD_GPU_UTILIZATION "nvidia-smi --query-gpu=utilization.gpu --format=csv"
#define NVSMI_CMD_MEM_UTILIZATION "nvidia-smi --query-gpu=utilization.memory,memory.total,memory.free,memory.used --format=csv"

#ifndef ICON_PATH
#define ICON_PATH ""
#endif

// enums are using for convenience and readability
// enums are using in array indexing

// nvidia-system-monitor processes columns
namespace NVSMColumns {
enum {
    Name,
    Type,
    GPUIdx,
    PID,
    FbMem,
    Sm,
    Mem,
    Enc,
    Dec,
    columns_count
};
}

typedef unsigned int uint;

constexpr uint NVSMMaxWorkers = 3;

#endif
