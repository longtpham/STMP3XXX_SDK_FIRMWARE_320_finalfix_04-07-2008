#include "types.h"

typedef struct MemoryRegionStruct {
    WORD m_wResourceLoaded;
    _P INT *m_StartAddress;
    _P INT *m_EndAddress;
} MemoryRegion;


//define each memory region
MemoryRegion *g_RegionTable[] = 
{
    NULL
};
