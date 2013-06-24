///////////////////////////////////////////////////////////////////////////////
//!
//! \file overlay_switchtotask.c
//! \brief
//!     This file implements the Kernel_SwitchToTask internal kernel function.
//!
//! Copyright (c) SigmaTel, Inc. Unpublished
//!
//! SigmaTel, Inc.
//! Proprietary  Confidential
//!
//! This source code and the algorithms implemented therein constitute
//! confidential information and may comprise trade secrets of SigmaTel, Inc.
//! or its associates, and any use thereof is subject to the terms and
//! conditions of the Confidential Disclosure Agreement pursuant to which this
//! source code was originally received.
//!
///////////////////////////////////////////////////////////////////////////////

#include "kernel.h"
#include "kernel_internal.h"

#pragma optimize 3

//This file overrides Kernel_SwitchToTask in kernel.mk and replaces it with the one that knows how to deal with overlays.


//=============================================================================
// Kernel_SwitchToTask
//-----------------------------------------------------------------------------
//! This call handles switching to a new task.
//!
//! \param pEntry
//!     This is the task entry pointer for the task to run.
//!
//! \retval
//!     None.
//!
//=============================================================================
_reentrant void Kernel_SwitchToTask(TaskEntry *pEntry)
{
    //
    // Do not switch to the kernel because it is not a task.
    //
    assert(KERNEL_OWNER != g_pCurrentTask);
    
    g_pCurrentTask = pEntry;
    Kernel_pTempStack = pEntry->m_pStackPointer;
    Kernel_ContextSwitch();
    pEntry->m_pStackPointer = Kernel_pTempStack;

}


// This was moved from overlay_startup.c to ensure that it remains in the system region
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



