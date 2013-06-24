#include "kernel.h"
#include "kernel_internal.h"

void _asmfunc SysSignal(SignalGroup *pGroup, int iGroup)
{
    *pGroup |= (1<<iGroup);
}