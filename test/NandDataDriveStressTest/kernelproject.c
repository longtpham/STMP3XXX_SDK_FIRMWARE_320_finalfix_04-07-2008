#include "kernel.h"
#include "overlay.h"
#include "resource.h"
#include <project.h>

extern TaskEntry g_DisplayTaskEntry;
extern CircularBufferDescriptor g_DisplayTaskQueueDescriptor;

//This is a sample Task configuration file

//The task entry points can be declared here, or from an include file.
_reentrant void Task0EntryPoint(void);

//declare the task stacks.  Each task must have a stack that is tuned to its needs.
WORD g_Task0Stack[500];

//declare the task message queues.  Each task muct have a queue that is tuned to its needs.
_circ WORD g_Task0MessageQueue[32];

CircularBufferDescriptor g_Task0QueueDescriptor={ g_Task0MessageQueue,g_Task0MessageQueue};

//declare each task as a task entry.
TaskEntry g_Task0Entry = {
                            &g_Task0QueueDescriptor,
                            NULL,
                            0,
                            EVENT_TIMEOUT,
                            g_Task0Stack,
                            0,
                            TIMEOUT_IMMEDIATE,
                            {0},
                            0,
                            g_Task0Stack,
                            500,
                            Task0EntryPoint
                         };

TaskEntry *g_TaskList[]={
                        &g_DisplayTaskEntry,
                        &g_Task0Entry,
                         NULL
                        };

//the routing table describes which messages need to go to which queues.
RoutingEntry g_RoutingTable[] = 
{
    {0x010000, &g_Task0QueueDescriptor},
    {0x030000, &g_DisplayTaskQueueDescriptor},
    NULL
};

//make a table of the memory regions.
MemoryRegion *g_RegionTable[] = 
{
    NULL
};


SyncObject g_Sync_UserScratchY;
_Y WORD g_wUserScratchY[SCRATCH_USER_Y_SIZE];

SyncObject g_Sync_UserScratchY;
_X WORD g_wUserScratchX[SCRATCH_USER_X_SIZE];

