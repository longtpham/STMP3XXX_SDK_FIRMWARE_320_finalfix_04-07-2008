
#include "kernel.h"
#include "kernel_internal.h"
#include "instrumentation.h"

#pragma optimize 3

_reentrant void Kernel_SwitchToTask(TaskEntry *pEntry)
{
    g_pCurrentTask = pEntry;
    Kernel_pTempStack = pEntry->m_pStackPointer;
#ifdef INSTRUMENT_CURRENT_TASK
    InstrumentSetValue(pEntry->m_wTaskID);
#endif
    Kernel_ContextSwitch();
#ifdef INSTRUMENT_CURRENT_TASK
    InstrumentSetValue(0);
#endif
    pEntry->m_pStackPointer = Kernel_pTempStack;

}

// This was moved from kernel_startup.c to ensure that it remains in the system region
// if needed during operation
_reentrant void UnexpectedTaskExit(void)
{
    assert(0);
}
    //A task should never return from its entry point function.   Ever.  
    //If you're here, its because one of your tasks returned when it shouldn't have.
    //Look back in the OnCE tracebuffer to see where you came from.  (At this point, entry #3 in the trace buffer 
    //display should have the address of the RTS of the task's entry point function in it.  Double click on this
    //entry to take you to the end of that function.
    //g_pCurrentTask points to the current TaskEntry that is active.  That will also be helpful.

