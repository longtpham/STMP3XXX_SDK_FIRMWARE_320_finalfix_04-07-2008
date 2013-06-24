#ifndef __OVERLAY_INTERNAL_H
#define __OVERLAY_INTERNAL_H

#include "kernel_internal.h"

extern TaskEntry *g_pTaskToLoad;
extern SignalGroup g_OverlaySignal;

_asmfunc MemoryRegion *Kernel_GetRegion(_P INT *pAddress);
_asmfunc BOOL Overlay_IsTaskLoaded(TaskEntry*pTask);


#endif 