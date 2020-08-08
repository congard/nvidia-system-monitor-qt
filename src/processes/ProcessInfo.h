#ifndef QNVSM_PROCESSINFO_H
#define QNVSM_PROCESSINFO_H

#include <string>

class ProcessInfo {
public:
    ProcessInfo(std::string inName, std::string type, std::string gpuIdx, std::string pid,
                std::string sm, std::string mem, std::string enc, std::string dec,
                std::string fbmem);

public:
    std::string name;
    std::string type; // C or G
    std::string gpuIdx, pid, sm, mem, enc, dec, fbmem; // integers
};

#endif //QNVSM_PROCESSINFO_H
