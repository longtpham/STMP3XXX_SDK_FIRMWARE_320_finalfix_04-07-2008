#include "types.h"
#include "resource.h"

typedef struct MemoryRegionStruct {
    WORD m_wResourceLoaded;
    _P INT *m_StartAddress;
    _P INT *m_EndAddress;
	WORD m_wSysCallFunctionCounter;
} MemoryRegion;


//define each memory region
extern _P INT _lc_u_FuncletRegion_b;
extern _P INT _lc_u_FuncletRegion_e;
MemoryRegion g_FuncletRegion = {0,&_lc_u_FuncletRegion_b,&_lc_u_FuncletRegion_e,0};


extern _P INT _lc_u_ModuleRegion_b;
extern _P INT _lc_u_ModuleRegion_e;
MemoryRegion g_ModuleRegion = {0,&_lc_u_ModuleRegion_b,&_lc_u_ModuleRegion_e,0};

extern _P INT _lc_u_MediaRegion_b;
extern _P INT _lc_u_MediaRegion_e;
MemoryRegion g_MediaRegion = {RSRC_NANDMEDIAINIT,&_lc_u_MediaRegion_b,&_lc_u_MediaRegion_e,0};
#ifdef DEVICE_3500
extern _P INT _lc_u_bP_app_cluster;
extern _P INT _lc_u_eP_app_cluster;
MemoryRegion g_AppRegion = {RSRC_CHECKDISK,&_lc_u_bP_app_cluster,&_lc_u_eP_app_cluster,0};
#endif



MemoryRegion *g_RegionTable[] = 
{
    &g_FuncletRegion,
    &g_MediaRegion,
#ifdef DEVICE_3500
    &g_AppRegion,
#endif
    &g_ModuleRegion,
    NULL
};
