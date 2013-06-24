////////////////////////////////////////////////////////////////////////////////
// Copyright (C) SigmaTel, Inc. 2002-2003
//
// Filename:     softtimers.c
// Description:  
// API to receive a msg m sent to API user after x ms from API call execution time. 
//                or cyclic messages at period "interval" milliseconds.
// This file's module init and process function pointers are in the ModuleTable
//  and are called by the cooperative executive. 
////////////////////////////////////////////////////////////////////////////////
#include "types.h"
#include "exec.h"
#include "messages.h"
#include "project.h"

#ifndef SOFT_TIMERS
#define SOFT_TIMERS 2
#endif // others in menus.h, project.inc/h redefines this first. 

#ifndef NULL
#define NULL 0
#endif

long _asmfunc SysGetCurrentTime(void);


struct Timer
{
    int m_bActive   :  1;
    int m_iCount    : 23;
    int m_iInterval;
    unsigned long m_ulTimeout;
    int m_iMsg;
};

struct ModuleEntry *g_pSoftTimerModuleEntry;


struct Timer glb_SoftTimers[SOFT_TIMERS];
//------------------------------------------------------------------------------
// Function: SoftTimerModuleInit
// Inputs:   Pointer to ModuleEntry structure
// Outputs:  A = wait flag that Executive will store in softtimer ModuleTableEntry.wait
//
// Notes:    r0 input param was corrected in sysmodule caller with merge 2.313.006
//
int _reentrant SoftTimerModuleInit(struct ModuleEntry *pModuleTableEntry)
{
    int i;
    union WaitMask returnMask;     

    g_pSoftTimerModuleEntry = pModuleTableEntry;

    for(i=0;i<SOFT_TIMERS;i++)
    {
        glb_SoftTimers[i].m_bActive=0;
    }

    //Tell ModuleTableEntry.wait to call this module's process function next when it has a msg
    returnMask.I=0x000000 | 2; // EVENT_MESSAGE
    return returnMask.I;  
}

//------------------------------------------------------------------------------
// Function: SoftTimerModuleProcessQueue()
// Inputs:   (union WaitMask Signals, struct MessageQueueDescriptor * pQueue)
//
//  API callers register for or cancel recurring or n-shot messages w/ these two calls:
//
//   SysPostMessage(6, SOFT_TIMER_SET_TIMER, TIMER_yourNameMacroFromMenus.h,
//                  iMsgCount or 0 for recurring, iInterval in ms, MENU_MSG_yourRequestedMsg);    
//
//   SysPostMessage(3, SOFT_TIMER_KILL_TIMER, TIMER_yourNameMacroFromMenus.h);
//
// Outputs:  int wait flags specifying when to call this module process function next.
//
// Notes:    All times are in milliseconds. Absolute times are ms since system start. 
//
// Let STIn be active SoftTimeoutInterval for nth timer (may be recurring)
// Let ST1...STn be SoftTimeout time for nth timer, in ms since bootup. STn=CurrentTime+STIn
// STmin = min of active ST1...STn    Could be set in a global only on registration event.
// Let MT be ModuleTimeout (time for Executive to call this module next). 
//                                                    Current Design: MT = STmin
// Simplifying, we have STmin in two usable forms:   
//                    STmin=CurrentTime+min(STIn)  or STmin = min(CurrentTime+STIn) 
//
// So in current design: MT=CurrentTime+min(STIn)  or    MT = min(CurrentTime+STIn)
//------------------------------------------------------------------------------
int _reentrant SoftTimerModuleProcessQueue(union WaitMask Signals,struct MessageQueueDescriptor * pQueue)
{
    union WaitMask returnMask; 
    struct CMessage  Msg;
    unsigned long    ulCurrentTime;
    unsigned long    ulNextTimeout;
    int i;

    ulCurrentTime = SysGetCurrentTime();

    returnMask.I=0x000000 | 2;//Initialize it to signal us if we have a message

    //handle incoming messages
    if(Signals.B.m_bMessage)
    {
        while(GetMessage(pQueue,&Msg))
        {
            int iTimer;
            struct Timer *pTimer;

            switch(Msg.m_uMsg[0])
            {
                case SOFT_TIMER_KILL_TIMER:
                    iTimer = Msg.m_uMsg[1];
                    if(iTimer<SOFT_TIMERS)
                    {
                        pTimer = &glb_SoftTimers[iTimer];
                        pTimer->m_bActive = 0;
                    }
                    else
                    {
                        //Timer out of range. 
                        SystemHalt(); // Resets SOC if RETAIL build.
                    }
                    break;

                case SOFT_TIMER_SET_TIMER:
                    iTimer = Msg.m_uMsg[1];
                    if(iTimer<SOFT_TIMERS)
                    {
                        pTimer = &glb_SoftTimers[iTimer];
                        pTimer->m_bActive = 1;
                        pTimer->m_iCount = Msg.m_uMsg[2]; // 0 for recurring
                        pTimer->m_iInterval = Msg.m_uMsg[3]; // in milliseconds
                        pTimer->m_ulTimeout = ulCurrentTime+Msg.m_uMsg[3];
                        pTimer->m_iMsg = Msg.m_uMsg[4];
                    }
                    else
                    {
                        //Timer out of range. 
                        SystemHalt(); // Resets SOC if RETAIL build.
                    }
                    break;

                default: 
                    break;
            }
        }
    }

    //check for and handle all active soft timers
    ulNextTimeout = MAX_ULONG; 

    for(i=0; i<SOFT_TIMERS; i++)
    {
        struct Timer *pTimer= &glb_SoftTimers[i];

        if(pTimer->m_bActive)
        {
            returnMask.B.m_bTimer=1;//add 'signal us if a timer message' to the signal mask
            if(ulCurrentTime >= pTimer->m_ulTimeout)
            {
                SysPostMessage(2,pTimer->m_iMsg);
                if(pTimer->m_iCount)
                {//It still has times to run left 
                    pTimer->m_iCount--; //decrement the count by one
                    if(pTimer->m_iCount)//any more times left?
                        pTimer->m_ulTimeout = ulCurrentTime + pTimer->m_iInterval;
                    else
                        pTimer->m_bActive = 0;
                }
                else
                {//its a reoccuring timeout, update the timeout
                    pTimer->m_ulTimeout = ulCurrentTime + pTimer->m_iInterval;
                }
            }

            //Find min soft timeout and sets softimer ModuleTableEntry's next run time
            if(ulNextTimeout > (pTimer->m_ulTimeout)) 
            {   //if the ulNextTimeout is greater than this timer's timeout value, we have a new min
                ulNextTimeout = pTimer->m_ulTimeout;
                g_pSoftTimerModuleEntry->m_uTimeOutHigh = ulNextTimeout>>24;
                g_pSoftTimerModuleEntry->m_uTimeOutLow = ulNextTimeout&0xffffff;
                // When main loop's DSP wait for irq instruction doesn't run due to either
                // wait instr removal or Executive mistakenly calling this soft timer  
                // or other module continuously, (instead of at next min soft timeout):
                // the Noise Floor improved 5dB during 0dB test (excludes harmonics). 
                // We kept main loop's wait instr to extend bat life by 5%. Audio remains in spec.
            }
        }
    } // end loop

    return returnMask.I; 
}

