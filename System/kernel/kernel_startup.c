#include "kernel.h"
#include "kernel_internal.h"
#include "icoll.h"

#pragma optimize 1

void StarterFunction(void);

_reentrant void Kernel_Startup(void)
{
    int x;
    TaskEntry **pEntry = g_TaskList;
    int iTaskID= 1;
    __asm (" move sr,%0" : "=D"(x));
    while(*pEntry)
    {
        int i;
        int stacksize = (*pEntry)->m_wStackSize;
        (*pEntry)->m_wTaskID = iTaskID++;
        for(i=0;i<stacksize;i++)
        {
            ((*pEntry)->m_pStackPointer[i]) = iTaskID;
        }
        *((*pEntry)->m_pStackPointer++) = (int)((*pEntry)->m_pEntryPoint);  //required by the starter function
        *((*pEntry)->m_pStackPointer++) = x;                                //SR register required by task switch
        *((*pEntry)->m_pStackPointer++) = (int)StarterFunction;             //SSH required by task switch
        *((*pEntry)->m_pStackPointer++) = 0;                                //Initial SP for the task
        pEntry++;
    }
    IcollResetVectorTable();
}

// UnexpectedTaskExit() moved to kernel_switchtotask.c
// As ptextkernel_startup will be moved to an area that may be overwritten during operation.


#pragma asm
    include "sysmacro.asm"
    extern FUnexpectedTaskExit

FStarterFunction
    move   #>FUnexpectedTaskExit,SSH;move the stack overflow address into the stack (need to prime it with something, or else _reentrant function
                                    ; prologue will underflow the hardware stack)
    nop                             ;pipeline
    pop     SSH                     ;get the real entry point and put it onto the hardware stack
    nop                             ;pipeline
    rts                             ;return back to the entry point...

#pragma endasm