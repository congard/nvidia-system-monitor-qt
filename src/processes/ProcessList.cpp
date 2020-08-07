#include "ProcessList.h"

using namespace std;

ProcessList::ProcessList(const string& in_name, const string& in_type,
                         const string& in_gpuIdx, const string& in_pid,
                         const string& in_sm, const string& in_mem,
                         const string& in_enc, const string& in_dec,
                         const string& in_fbmem)
{
    name = in_name;
    type = in_type;
    gpuIdx = in_gpuIdx;
    pid = in_pid;
    sm = in_sm;
    mem = in_mem;
    enc = in_enc;
    dec = in_dec;
    fbmem = in_fbmem;
}
