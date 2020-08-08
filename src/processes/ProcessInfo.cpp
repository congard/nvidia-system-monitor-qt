#include "ProcessInfo.h"

#include <utility>

using namespace std;

ProcessInfo::ProcessInfo(string in_name, string in_type, string in_gpuIdx, string in_pid,
                         string in_sm, string in_mem, string in_enc, string in_dec,
                         string in_fbmem)
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
