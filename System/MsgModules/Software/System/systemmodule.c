#include "types.h"
#include "exec.h"
#include "messages.h"
#include "project.h"
#include "batterycharge.h"

_asmfunc SystemShutdownFinal(void);
long _asmfunc SysGetCurrentTime(void);

struct ModuleEntry *g_pSystemModuleEntry;

int _reentrant SystemModuleInit(struct ModuleEntry *pEntry)
{
    int i;
    union WaitMask returnMask;
    DWORD dwNextTimeout;

    returnMask.I = 0; 
    g_pSystemModuleEntry=pEntry;

    returnMask.B.m_bMessage = TRUE;
    returnMask.B.m_bTimer   = TRUE;
    dwNextTimeout = SysGetCurrentTime() + 100;
    g_pSystemModuleEntry->m_uTimeOutHigh = dwNextTimeout>>24;
    g_pSystemModuleEntry->m_uTimeOutLow  = dwNextTimeout&0xffffff;
    return returnMask.I;
}


int _reentrant SystemModuleProcessQueue(union WaitMask Signals,struct MessageQueueDescriptor * pQueue)
{
    int i;
    union WaitMask returnMask; 
    struct CMessage  Msg;
    unsigned long    lCurrentTime;
    unsigned long    lNextTimeout;

    lCurrentTime = SysGetCurrentTime();
    returnMask.I = 0;
    //handle incoming messages
    if(Signals.B.m_bMessage)
    {
        while(GetMessage(pQueue,&Msg))
        {
            int iTimer;
            struct Timer *pTimer;

            switch(Msg.m_uMsg[0])
            {
                case SYSTEM_SHUTDOWN_FINAL:
                    SystemShutdownFinal();
                    break;
                default: 
                    break;
            }
        }
    }
    if(Signals.B.m_bTimer)
    {
//'Heatbeat' functions can be added here. 
    }
    lCurrentTime += 100;
    g_pSystemModuleEntry->m_uTimeOutHigh = lCurrentTime>>24;
    g_pSystemModuleEntry->m_uTimeOutLow  = lCurrentTime&0xffffff;

    returnMask.B.m_bMessage = TRUE;
    // Disable the timer to reduce NAND access if using STFM1000
#ifndef TUNER_STFM1000    
    returnMask.B.m_bTimer   = TRUE;
#endif
    return returnMask.I;
}
