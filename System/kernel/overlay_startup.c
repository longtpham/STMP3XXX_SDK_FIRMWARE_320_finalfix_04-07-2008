#include "overlay.h"
#include "overlay_internal.h"
#include "icoll.h"

#pragma optimize 1

void StarterFunction(void);

_reentrant void Kernel_Startup(void)
{
    MemoryRegion **pRegion = g_RegionTable;
    int SavedSR,i;
    TaskEntry **pEntry = g_TaskList;
    WORD *pwResourceTable;
    __asm (" move sr,%0" : "=D"(SavedSR));

    while(*pRegion)
    {
        if((*pRegion)->m_wResourceLoaded ==0)//if the region is static
        {
            for(i=(int)(*pRegion)->m_StartAddress;i<(int)(*pRegion)->m_EndAddress;i++)
                (*((_P int *)i))=0x200;
        }
        pRegion++;
    }

    while(*pEntry)
    {
        int iTaskID= (*pEntry)->m_wTaskID;
        int i;
        int stacksize = (*pEntry)->m_wStackSize;
        for(i=0;i<stacksize;i++)
            {
            ((*pEntry)->m_pStackPointer[i]) = iTaskID;
            }
        pwResourceTable = (*pEntry)->m_wRegionResources;
        pRegion = g_RegionTable;
        while(*pRegion)
        {
            if((*pRegion)->m_wResourceLoaded == REGION_STATIC)//if the region is static
                *pwResourceTable = 0;//then we don't care if its loaded or not.
            pwResourceTable++;
            pRegion++;
        }
        *((*pEntry)->m_pStackPointer++) = (int)((*pEntry)->m_pEntryPoint);  //required by the starter function
        *((*pEntry)->m_pStackPointer++) = SavedSR;                          //SR register required by task switch
        *((*pEntry)->m_pStackPointer++) = (int)StarterFunction;             //SSH required by task switch
        *((*pEntry)->m_pStackPointer++) = 0;                                //Initial SP for the task

        pEntry++;
    }
    IcollResetVectorTable();
}

// UnexpectedTaskExit() moved to overlay_switchtotask.c
// As ptextoverlay_startup will be moved to an area that may be overwritten during operation.

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