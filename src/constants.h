#ifndef CONSTANTS_H
#define CONSTANTS_H

#define NVSM_CONF_UPDATE_DELAY "updateDelay"
#define NVSM_CONF_GRAPH_LENGTH "graphLength"
#define NVSM_CONF_GCOLOR "gpuColor"

#define NVSMI_CMD_GPU_COUNT "nvidia-smi --query-gpu=count --format=csv"
#define NVSMI_CMD_PROCESSES "nvidia-smi pmon -c 1 -s m"
#define NVSMI_CMD_GPU_UTILIZATION "nvidia-smi --query-gpu=utilization.gpu --format=csv"
#define NVSMI_CMD_MEM_UTILIZATION "nvidia-smi --query-gpu=utilization.memory,memory.total,memory.free,memory.used --format=csv"

// nvidia-smi command output indices
#define NVSMI_GPUIDX 0
#define NVSMI_PID    1
#define NVSMI_TYPE   2
#define NVSMI_MEM    3
#define NVSMI_NAME   4

// nvidia-system-monitor processes columns
#define NVSM_GPUIDX 2
#define NVSM_PID    3
#define NVSM_TYPE   1
#define NVSM_MEM    4
#define NVSM_NAME   0

#define GRAPTH_OFFSET               32
#define STATUS_OBJECT_OFFSET        16
#define STATUS_OBJECT_TEXT_OFFSET   16

#define NVSM_WORKERS_MAX 3

#ifndef ICON_PATH
#define ICON_PATH ""
#endif

typedef unsigned int uint;

#endif
