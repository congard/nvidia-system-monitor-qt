#ifndef QNVSM_CONSTANTS_H
#define QNVSM_CONSTANTS_H

#define NVSM_CONF_UPDATE_DELAY "updateDelay"
#define NVSM_CONF_GRAPH_LENGTH "graphLength"
#define NVSM_CONF_GCOLOR "gpuColor"

#define NVSMI_CMD_GPU_COUNT "nvidia-smi --query-gpu=count --format=csv"
#define NVSMI_CMD_PROCESSES "nvidia-smi pmon -c 1 -s um"
#define NVSMI_CMD_GPU_UTILIZATION "nvidia-smi --query-gpu=utilization.gpu --format=csv"
#define NVSMI_CMD_MEM_UTILIZATION "nvidia-smi --query-gpu=utilization.memory,memory.total,memory.free,memory.used --format=csv"

#ifndef ICON_PATH
#define ICON_PATH ""
#endif

// enums are using for convenience and readability
// enums are using in array indexing

// nvidia-smi process output indices
namespace NVSMIProcess {
enum {
    GPUIdx,
    PID,
    Type,
    Sm,
    Mem,
    Enc,
    Dec,
    FbMem,
    Name
};
}

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

constexpr uint GraphOffset = 32;
constexpr uint StatusObjectOffset = 16;
constexpr uint StatusObjectTextOffset = 16;

constexpr uint NVSMMaxWorkers = 3;

#endif
