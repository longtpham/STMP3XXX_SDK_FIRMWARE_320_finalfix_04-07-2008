#include "overlay.h"
#include "overlay_internal.h"
#include "queues.h"
#include "sysresourceapi.h"

//This file overrides Kernel_IsTaskReady in kernel.mk and replaces it with the one that knows how to deal with overlays.

TaskEntry *g_pTaskToLoad = NULL;
SignalGroup g_OverlaySignal;
//#pragma optimize 3
_reentrant WORD Kernel_IsTaskReady(TaskEntry* pEntry)
{
    WORD rtn = FALSE;
    if(pEntry->m_lTimeout <= GetTime())
    {
        rtn = TRUE;
    }
    else
    {
        switch(pEntry->m_wState)
        {
        case EVENT_OBJECT:
            assert (pEntry->m_pWaitingOnPtr);
            {
                TaskEntry *tmp = ((SyncObject*)(pEntry->m_pWaitingOnPtr))->m_pOwner;
                if(!tmp || pEntry == tmp)
                    rtn = TRUE;
            }
            break;
        case EVENT_MESSAGE:
            rtn = Kernel_IsMessageAvailable(pEntry);
            break;
        case EVENT_QUEUE:
            assert (pEntry->m_pWaitingOnPtr);
            rtn = QueueGetEmptySpace(pEntry->m_pWaitingOnPtr)>pEntry->m_wWaitingOnWord;
            break;
        case EVENT_SIGNAL:
            rtn = ((*((WORD*)pEntry->m_pWaitingOnPtr)) & (1<<pEntry->m_wWaitingOnWord)) ? TRUE : FALSE;
            break;
        case EVENT_RELEASING_OBJECT2://this interim state is to slow down the task thats releasing the object, so that it can't
                                    //grab it before the kernel loads up another task who may be waiting on it.
            pEntry->m_wState = EVENT_RELEASING_OBJECT1;
            break;
        case EVENT_RELEASING_OBJECT1://this interim state is to slow down the task thats releasing the object, so that it can't
                                    //grab it before the kernel loads up another task who may be waiting on it.
            pEntry->m_wState = EVENT_TIMEOUT;
            break;

        }
    }
    if(rtn)
    {
        if(!Overlay_IsTaskLoaded(pEntry))
        {//if the task is otherwise ready, lets load it
            rtn = FALSE;
            if(g_pTaskToLoad)
            {//if we've already got a task to load, lets make sure its a lower priority before overwriting it.
                if(pEntry->m_wPriority > g_pTaskToLoad->m_wPriority)
                    g_pTaskToLoad = pEntry;
            }
            else
                g_pTaskToLoad = pEntry;
            SysSignal(&g_OverlaySignal, 1);//signal the overlay group so that the overlay manager will run.
        }
    }
    return rtn;
}