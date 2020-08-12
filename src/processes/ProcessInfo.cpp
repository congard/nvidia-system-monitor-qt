#include "ProcessInfo.h"

#include <utility>

using namespace std;

ProcessInfo::ProcessInfo(QString in_name, QString in_type, QString in_gpuIdx, QString in_pid,
                         QString in_sm, QString in_mem, QString in_enc, QString in_dec,
                         QString in_fbmem)
        : name(move(in_name)),
          type(move(in_type)),
          gpuIdx(move(in_gpuIdx)),
          pid(move(in_pid)),
          sm(move(in_sm)),
          mem(move(in_mem)),
          enc(move(in_enc)),
          dec(move(in_dec)),
          fbmem(move(in_fbmem))
{
    // empty, see initializer list above
}
