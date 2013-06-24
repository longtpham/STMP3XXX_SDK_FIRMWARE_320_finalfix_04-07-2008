#ifndef _KERNEL_INTERNAL
#define _KERNEL_INTERNAL

extern WORD g_KernelStack[];
extern TaskEntry *g_pCurrentTask;
extern WORD *Kernel_pTempStack;
extern _L LONG g_lTimerTick;

_reentrant TaskEntry * Kernel_GetNextTask(void);
_reentrant void Kernel_SwitchToTask(TaskEntry*);
_asmfunc void Kernel_ContextSwitch(void );
_reentrant void Kernel_SetTimeout(INT wTimeout);
_reentrant void Kernel_IncrementTick(void);
_asmfunc WORD Kernel_AddMessageToQueue(CircularBufferDescriptor*, Message*);
_reentrant WORD Kernel_IsTaskReady(TaskEntry *);
_asmfunc INT Kernel_IsMessageAvailable(TaskEntry *);
_reentrant WORD Kernel_IsSystemAlive(void);
_reentrant void Kernel_TickleWatchdog(void);
_reentrant void Kernel_Heartbeat(void);


#endif