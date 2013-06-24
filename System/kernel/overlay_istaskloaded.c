#include "overlay.h"
#include "overlay_internal.h"

BOOL _asmfunc Overlay_IsTaskLoaded(TaskEntry *pEntry)
{//this traces back the stack of the task at 'pEntry' to see if the 
    MemoryRegion **pRegionTable=g_RegionTable;
    WORD *pwTableValue = pEntry->m_wRegionResources;
    BOOL rtn = TRUE;
    while(*pRegionTable)
    {
        if(*pwTableValue)
        {//we only care if the value is non-zero (means a resource is required for that particular region)
            if(pRegionTable[0]->m_wResourceLoaded != *pwTableValue)
            {//see if the first region matches the required value
                rtn = FALSE;
                break;
            }
        }
        pRegionTable++;//increment the region
        pwTableValue++;//increment the table index
    }
    return rtn;
}
