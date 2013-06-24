#include "overlay.h"

//define each memory region
extern _P INT _lc_u_Region1_b;
extern _P INT _lc_u_Region1_e;
MemoryRegion g_Region1 = {0,&_lc_u_Region1_b,&_lc_u_Region1_e};


extern _P INT _lc_u_Region2_b;
extern _P INT _lc_u_Region2_e;
MemoryRegion g_Region2 = {0,&_lc_u_Region2_b,&_lc_u_Region2_e};

extern _P INT _lc_u_Region3_b;
extern _P INT _lc_u_Region3_e;
MemoryRegion g_Region3 = {(WORD)-1,&_lc_u_Region3_b,&_lc_u_Region3_e};

//make a table of the memory regions.
MemoryRegion *g_RegionTable[] = 
{
    &g_Region1,
    &g_Region2,
    &g_Region3,
    NULL
};
