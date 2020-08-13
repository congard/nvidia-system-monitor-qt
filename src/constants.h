#ifndef QNVSM_CONSTANTS_H
#define QNVSM_CONSTANTS_H

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
