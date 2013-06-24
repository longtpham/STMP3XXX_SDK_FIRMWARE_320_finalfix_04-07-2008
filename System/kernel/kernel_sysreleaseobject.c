
#include "kernel.h"
#include "kernel_internal.h"

#pragma optimize 1

_reentrant void SysReleaseObject(SyncObject *pObject)
{
    if(g_pCurrentTask)
    {
        assert(pObject->counter.m_iOwner==(int)g_pCurrentTask);
        if(pObject->counter.m_iCount)
        {
            pObject->counter.m_iCount--;
        }
        else
        {    
        pObject->m_pOwner  = NULL;
        }
        Kernel_SetTimeout(0);
        Kernel_ContextSwitch();        
        g_pCurrentTask->m_wState = EVENT_RELEASING_OBJECT2;
    }
}