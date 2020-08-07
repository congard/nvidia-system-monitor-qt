#ifndef QNVSM_PROCESSLIST_H
#define QNVSM_PROCESSLIST_H

#include <string>

class ProcessList {
public:
    ProcessList(const std::string &inName, const std::string &type,
                const std::string &gpuIdx, const std::string &pid,
                const std::string &sm, const std::string &mem,
                const std::string &enc, const std::string &dec,
                const std::string &fbmem);

public:
    std::string name;
    std::string type; // C or G
    std::string gpuIdx, pid, sm, mem, enc, dec, fbmem; // integers
};

#endif //QNVSM_PROCESSLIST_H
