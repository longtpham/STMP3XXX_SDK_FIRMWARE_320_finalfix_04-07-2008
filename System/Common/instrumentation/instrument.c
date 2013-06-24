////////////////////////////////////////////////////////////////////////////////
// Copyright(C) SigmaTel, Inc. 2003-
//
// Filename: instrument.c
// Description:  Used as instrumentation to control the oscilliscope
////////////////////////////////////////////////////////////////////////////////
#include "types.h"
#include "project.h"
#include "regsgpio.h"
#include "sysirq.h"

#include "instrumentation.h"

#pragma optimize 1

#ifndef MAX_INSTRUMENTATION_CONTEXTS
#define MAX_INSTRUMENTATION_CONTEXTS 20
#endif

BYTE g_btEvents=0;

typedef struct _ContextEntry
{
    unsigned int m_wValue   : 8;
    unsigned int m_wContext : 8;
    unsigned int m_wEvents  : 8;
} ContextEntry;



int g_ContextStackIndex = 0;
ContextEntry g_ContextStack[MAX_INSTRUMENTATION_CONTEXTS];


////////////////////////////////////////////////////////////////////////////////
//
//>  Name:          InstrumentToggleClock
//
//   Type:          Function
//
//   Description:   Toggles the Clock state
//
//   Inputs:        none
//
//   Outputs:       void
//
//   Notes:         
//<
////////////////////////////////////////////////////////////////////////////////
_reentrant _callee_save  void InstrumentToggleClock(void)
{
#pragma asm
        include "sysmacro.asm"
        include "regsgpio.inc"
            push x0
        move    x:HW_GP3DOR,x0
        bset    #15,x0
        move    x0,x:HW_GP3DOR
        pop x0
#pragma endasm
}


////////////////////////////////////////////////////////////////////////////////
//
//>  Name:          InstrumentUpdate
//
//   Type:          Function
//
//   Description:   Updates the GPIO pins with the current instrument information
//
//   Inputs:        none
//
//   Outputs:       void
//
//   Notes:         
//<
////////////////////////////////////////////////////////////////////////////////
_reentrant _callee_save void InstrumentUpdate(void)
{
    WORD oldIrq;
    union {
        ContextEntry Entry;
        WORD         W;
    } Context;
    //block interrupts
    
    oldIrq = SysGetIrqLevel();
    SysSetIrqLevel(0x0300);
    InstrumentToggleClock();
    
    Context.Entry = g_ContextStack[g_ContextStackIndex];
    Context.Entry.m_wEvents = g_btEvents;
    HW_GP3DOR.I = Context.W;

    InstrumentToggleClock();
    //unblock interrupts
    SysSetIrqLevel(oldIrq);
}



////////////////////////////////////////////////////////////////////////////////
//
//>  Name:          InstrumentPushContext
//
//   Type:          Function
//
//   Description:   Pushes a new context onto the instrumentation stack.  This context 
//                  is the new currently visible context.   When PopContext is called, the
//                  old context is returned to being the currently visible context.
//
//   Inputs:        BYTE btContext      New context 
//   Inputs:        BYTE btValue        New value
//
//   Outputs:       void
//
//   Notes:         
//<
////////////////////////////////////////////////////////////////////////////////
_reentrant _callee_save void InstrumentPushContext(BYTE btContext, BYTE btValue)
{
    ContextEntry NewContext;
    NewContext.m_wValue = btValue;
    NewContext.m_wContext= btContext;
    g_ContextStackIndex++;
    if(g_ContextStackIndex < MAX_INSTRUMENTATION_CONTEXTS)
        g_ContextStack[g_ContextStackIndex]= NewContext;
    else
        SystemHalt();
    InstrumentUpdate();
}


////////////////////////////////////////////////////////////////////////////////
//
//>  Name:          InstrumentPopContext
//
//   Type:          Function
//
//   Description:   Pops off the last context and value
//
//   Inputs:        void
//
//   Outputs:       void
//
//   Notes:         
//<
////////////////////////////////////////////////////////////////////////////////
_reentrant _callee_save void InstrumentPopContext(void)
{
    g_ContextStackIndex--;
    if(g_ContextStackIndex < 0)
        SystemHalt();
    InstrumentUpdate();
}

////////////////////////////////////////////////////////////////////////////////
//
//>  Name:          InstrumentSetValue
//
//   Type:          Function
//
//   Description:   Sets the current value of the context, replacing the old one
//
//   Inputs:        BYTE btValue        New value
//
//   Outputs:       void
//
//   Notes:         
//<
////////////////////////////////////////////////////////////////////////////////
_reentrant _callee_save void InstrumentSetValue(BYTE btValue)
{
    g_ContextStack[g_ContextStackIndex].m_wValue = btValue;
    InstrumentUpdate();
}


////////////////////////////////////////////////////////////////////////////////
//
//>  Name:          InstrumentSetEvent
//
//   Type:          Function
//
//   Description:   Set the event pin for the numbered event
//
//   Inputs:        BYTE btEvent        Event to set
//
//   Outputs:       void
//
//   Notes:         
////////////////////////////////////////////////////////////////////////////////
_reentrant _callee_save void InstrumentSetEvent(BYTE btEvent)
{
    g_btEvents |= (1<<btEvent);
    InstrumentUpdate();
}

////////////////////////////////////////////////////////////////////////////////
//
//>  Name:          InstrumentClearEvent
//
//   Type:          Function
//
//   Description:   Clears the event pin for the numbered event
//
//   Inputs:        BYTE btEvent        Event to clear
//
//   Outputs:       void
//
//   Notes:         
//<
////////////////////////////////////////////////////////////////////////////////
_reentrant _callee_save void InstrumentClearEvent(BYTE btEvent)
{
    g_btEvents &= ~(1<<btEvent);
    InstrumentUpdate();
}

////////////////////////////////////////////////////////////////////////////////
//
//>  Name:          InstrumentToggleEvent
//
//   Type:          Function
//
//   Description:   Toggle the event pin for the numbered event
//
//   Inputs:        BYTE btEvent        Event to toggle
//
//   Outputs:       void
//
//   Notes:         
//<
////////////////////////////////////////////////////////////////////////////////
_reentrant _callee_save void InstrumentToggleEvent(BYTE btEvent)
{
    g_btEvents ^= (1<<btEvent);
    InstrumentUpdate();
}

_reentrant _callee_save void InstrumentInitialize(void)
{
    HW_GP3ENR.I = (int) 0xffffff;
    HW_GP3DOER.I = (int) 0xffffff;
    HW_GP3PWR.I = (int) 0xffffff;
}
