#include "kernel.h"
#include "kernel_internal.h"

#pragma optimize 3
_reentrant TaskEntry *Kernel_GetNextTask(void)
{
    TaskEntry *pNextTask = NULL;
    TaskEntry **pNextTaskPtr=g_TaskList;

    //find highest priority task that is ready to run
    //This algorithm assumes that the list of TaskEntry's is sorted by priority (highest first)

    while(*pNextTaskPtr)
    {
        if(Kernel_IsTaskReady(*pNextTaskPtr))
        {
            pNextTask = *pNextTaskPtr;
            break;//if the task is ready, break out of the loop
        }
        pNextTaskPtr++;
    }

    //to avoid one task hogging the processor from other tasks of the same priorty, 
    //we will move that task after all other tasks of the same priority.
    if(*pNextTaskPtr)
    {
        while(*(pNextTaskPtr+1))
        {
            if((*pNextTaskPtr)->m_wPriority <= (*(pNextTaskPtr+1))->m_wPriority)
            {
                TaskEntry *pTempTaskPtr = *pNextTaskPtr;
                *pNextTaskPtr = *(pNextTaskPtr+1);
                *(pNextTaskPtr+1) = pTempTaskPtr;
                pNextTaskPtr++;
            }
            else
            {
                break;
            }
        }
    }
    return pNextTask;
}