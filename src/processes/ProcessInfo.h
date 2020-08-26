#ifndef QNVSM_PROCESSINFO_H
#define QNVSM_PROCESSINFO_H

#include <QString>

class ProcessInfo {
public:
    ProcessInfo();
    ProcessInfo(QString inName, QString type, QString gpuIdx, QString pid,
                QString sm, QString mem, QString enc, QString dec,
                QString fbmem);

public:
    QString name;
    QString type; // C or G
    QString gpuIdx, pid, sm, mem, enc, dec, fbmem; // integers
};

#endif //QNVSM_PROCESSINFO_H
