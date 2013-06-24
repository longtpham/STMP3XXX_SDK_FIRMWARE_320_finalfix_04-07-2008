error

Shouldn't be used as USBMSC does not use overlays (yet?)

#include "types.h"
#include "resource.h"

typedef struct MemoryRegionStruct {
    WORD m_wResourceLoaded;
    _P INT *m_StartAddress;
    _P INT *m_EndAddress;
} MemoryRegion;

//define each memory region

extern _P INT _lc_u_bP_sys_cluster;
extern _P INT _lc_u_eP_sys_cluster;
MemoryRegion g_SysRegion = {0xFFFFFF,&_lc_u_bP_sys_cluster,&_lc_u_eP_sys_cluster};

extern _P INT _lc_u_MediaRegion_b;                 
extern _P INT _lc_u_MediaRegion_e;
MemoryRegion g_MediaRegion = {RSRC_NANDMEDIAINIT,&_lc_u_MediaRegion_b,&_lc_u_MediaRegion_e};

MemoryRegion *g_RegionTable[] = 
{
    &g_MediaRegion,
    &g_SysRegion,
    NULL
};

#pragma asm
    org p,".filler":2
    ds $2000
#pragma endasm
