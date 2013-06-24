
#include "kernel.h"
#include "kernel_internal.h"

// The periodic ChipUpdateCapability() call is not desired in usbmsc or mtp but 
// is desired if this kernel is used for the player.
#ifdef STMP_BUILD_PLAYER
extern void ChipUpdateCapability(void);
#else
#define ChipUpdateCapability(void);
#endif 

#define CAP_CHECK_COUNTER_MAX 1000
WORD g_wCapCheckCounter;

#pragma optimize 3
_reentrant void Kernel_MainLoop(void)
{
    while(1)
    {
        TaskEntry *pEntry; 
        pEntry = Kernel_GetNextTask();
        if(pEntry)
        {
            Kernel_SwitchToTask(pEntry);
        }
        else
        {
            //set the processor into wait mode
        }
        if(Kernel_IsSystemAlive())
            Kernel_TickleWatchdog();
        Kernel_Heartbeat();

		// Every CAP_CHECK_COUNTER_MAX times, refresh hardware capabilities

		if(++g_wCapCheckCounter >= CAP_CHECK_COUNTER_MAX)
		{
			g_wCapCheckCounter = 0;
			ChipUpdateCapability();
		}
    }
}