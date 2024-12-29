#ifndef QNVSM_PROCESSINFO_H
#define QNVSM_PROCESSINFO_H

#include <QString>

struct ProcessInfo {
    // strings
    QString name;
    QString type; // C or G

    // integers
    QString gpuIdx;
    QString pid;
    QString sm;
    QString mem;
    QString enc;
    QString dec;
    QString fbmem;
};

#endif //QNVSM_PROCESSINFO_H
