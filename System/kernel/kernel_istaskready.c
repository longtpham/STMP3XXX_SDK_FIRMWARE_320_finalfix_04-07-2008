#include "kernel.h"
#include "kernel_internal.h"
#include "queues.h"

#pragma optimize 3
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
        case EVENT_RELEASING_OBJECT1://this interim state is to slow down the task thats releasing the object, so that it can't
        case EVENT_RELEASING_OBJECT2://this interim state is to slow down the task thats releasing the object, so that it can't
                                    //grab it before the kernel loads up another task who may be waiting on it.
            pEntry->m_wState = EVENT_TIMEOUT;
            break;

        }
    }

    return rtn;
}