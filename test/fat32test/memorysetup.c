#include "types.h"
#include "resource.h"

typedef struct MemoryRegionStruct {
    WORD m_wResourceLoaded;
    _P INT *m_StartAddress;
    _P INT *m_EndAddress;
} MemoryRegion;



extern _P INT _lc_u_MediaRegion_b;
extern _P INT _lc_u_MediaRegion_e;
MemoryRegion g_MediaRegion = {RSRC_NANDMEDIAINIT,&_lc_u_MediaRegion_b,&_lc_u_MediaRegion_e};



MemoryRegion *g_RegionTable[] = 
{
    &g_MediaRegion
};


//------------------------------------------------------------------------------
/**
// @todo -document this!  it has to be in *every* build that uses it!
// and that means cleaning this up so every app gets the change... somehow...
**/
//------------------------------------------------------------------------------
void region_table_discard_current_resource(void);
void region_table_discard_current_resource()
{
  g_MediaRegion.m_wResourceLoaded = 0;
}
//---eof------------------------------------------------------------------------

