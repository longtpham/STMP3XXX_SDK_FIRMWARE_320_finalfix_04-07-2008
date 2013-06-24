// filename: \test\kernel\sample.c
#include "kernel.h"
#include "overlay.h"
#include "resource.h"
#include <project.h>
#include "ddildl.h"
#include "sysresourceapi.h"
#include "messages.h"

extern TaskEntry g_DisplayTaskEntry;
extern CircularBufferDescriptor g_DisplayTaskQueueDescriptor;
//This is a sample Task configuration file

//The task entry points can be declared here, or from an include file.
_reentrant void Task0EntryPoint(void);
_reentrant void Task1EntryPoint(void);
_reentrant void Task2EntryPoint(void);
_reentrant void Task3EntryPoint(void);
_reentrant void Task4EntryPoint(void);


#define TASK0_STACK_SIZE    100
#define TASK1_STACK_SIZE    100
#define TASK2_STACK_SIZE    100
#define TASK3_STACK_SIZE    100
#define TASK4_STACK_SIZE    100

//declare the task stacks.  Each task must have a stack that is tuned to its needs.
WORD g_Task0Stack[TASK0_STACK_SIZE];
WORD g_Task1Stack[TASK1_STACK_SIZE];
WORD g_Task2Stack[TASK2_STACK_SIZE];
WORD g_Task3Stack[TASK3_STACK_SIZE];
WORD g_Task4Stack[TASK4_STACK_SIZE];

#define TASK0_QUEUE_SIZE    32
#define TASK1_QUEUE_SIZE    64
#define TASK2_QUEUE_SIZE    16
#define TASK3_QUEUE_SIZE    32
#define TASK4_QUEUE_SIZE    32

//declare the task message queues.  Each task muct have a queue that is tuned to its needs.
_circ WORD g_Task0MessageQueue[TASK0_QUEUE_SIZE];
_circ WORD g_Task1MessageQueue[TASK1_QUEUE_SIZE];
_circ WORD g_Task2MessageQueue[TASK2_QUEUE_SIZE];
_circ WORD g_Task3MessageQueue[TASK3_QUEUE_SIZE];
_circ WORD g_Task4MessageQueue[TASK4_QUEUE_SIZE];

CircularBufferDescriptor g_Task0QueueDescriptor={g_Task0MessageQueue,g_Task0MessageQueue};
CircularBufferDescriptor g_Task1QueueDescriptor={g_Task1MessageQueue,g_Task1MessageQueue};
CircularBufferDescriptor g_Task2QueueDescriptor={g_Task2MessageQueue,g_Task2MessageQueue};
CircularBufferDescriptor g_Task3QueueDescriptor={g_Task3MessageQueue,g_Task3MessageQueue};
CircularBufferDescriptor g_Task4QueueDescriptor={g_Task4MessageQueue,g_Task4MessageQueue};

//declare each task as a task entry.
TaskEntry g_Task0Entry = {
                            &g_Task0QueueDescriptor,            //Message queue
                            NULL,                               //Waiting on ptr
                            0,                                  //waiting on value
                            EVENT_TIMEOUT,                      //Initial State
                            g_Task0Stack,                       //current stack ptr
                            0,                                  //Task ID
                            TIMEOUT_IMMEDIATE,                  //Initial timeout value
                            {RSRC_REGION1_TASK0_BANK1,0,0},     //Required regions (none-required)
                            0,                                  //Priority (make it higher than any others)
                            g_Task0Stack,                       //initial stack pointer
                            TASK0_STACK_SIZE,                   //stack size
                            Task0EntryPoint                     //initial entry point
                         };
                            
TaskEntry g_Task1Entry = {
                            &g_Task1QueueDescriptor,            //Message queue
                            NULL,                               //Waiting on ptr
                            0,                                  //waiting on value
                            EVENT_TIMEOUT,                      //Initial State
                            g_Task1Stack,                       //current stack ptr
                            1,                                  //Task ID
                            TIMEOUT_IMMEDIATE,                  //Initial timeout value
                            {RSRC_REGION1_TASK1_BANK1,0,0},     //Required regions (none-required)
                            0,                                  //Priority (make it higher than any others)
                            g_Task1Stack,TASK1_STACK_SIZE,      //initial stack pointer
                            Task1EntryPoint                     //stack size
                         };                                     //initial entry point
                         
TaskEntry g_Task2Entry = {
                            &g_Task2QueueDescriptor,            //Message queue
                            NULL,                               //Waiting on ptr
                            0,                                  //waiting on value
                            EVENT_TIMEOUT,                      //Initial State
                            g_Task2Stack,                       //current stack ptr
                            2,                                  //Task ID
                            TIMEOUT_IMMEDIATE,                  //Initial timeout value
                            {RSRC_REGION1_TASK2_BANK1,0,0},     //Required regions (none-required)
                            0,                                  //Priority (make it higher than any others)
                            g_Task2Stack,                       //initial stack pointer
                            TASK2_STACK_SIZE,                   //stack size
                            Task2EntryPoint                     //initial entry point
                         };
                         
TaskEntry g_Task3Entry = {
                            &g_Task3QueueDescriptor,            //Message queue
                            NULL,                               //Waiting on ptr
                            0,                                  //waiting on value
                            EVENT_TIMEOUT,                      //Initial State
                            g_Task3Stack,                       //current stack ptr
                            3,                                  //Task ID
                            TIMEOUT_IMMEDIATE,                  //Initial timeout value
                            {0,RSRC_REGION2_TASK3_BANK1,0},     //Required regions (none-required)
                            0,                                  //Priority (make it higher than any others)
                            g_Task3Stack,                       //initial stack pointer
                            TASK3_STACK_SIZE,                   //stack size
                            Task3EntryPoint                     //initial entry point
                         };
                         
TaskEntry g_Task4Entry = {
                            &g_Task4QueueDescriptor,            //Message queue 
                            NULL,                               //Waiting on ptr
                            0,                                  //waiting on value
                            EVENT_TIMEOUT,                      //Initial State
                            g_Task4Stack,                       //current stack ptr
                            4,                                  //Task ID
                            TIMEOUT_IMMEDIATE,                  //Initial timeout value
                            {0,RSRC_REGION2_TASK4_BANK1,0},     //Required regions (none-required)
                            0,                                  //Priority (make it higher than any others)
                            g_Task4Stack,                       //initial stack pointer
                            TASK4_STACK_SIZE,                   //stack size
                            Task4EntryPoint                     //initial entry point
                         };


TaskEntry *g_TaskList[]={
                         &g_OverlayManagerTaskEntry ,           //required for overlays
                         &g_Task0Entry,
                         &g_Task1Entry,
                         &g_Task2Entry,
                         &g_Task3Entry,
                         &g_Task4Entry,
                         &g_DisplayTaskEntry,
                         NULL
                        };

//the routing table describes which messages need to go to which queues.
RoutingEntry g_RoutingTable[] = 
{
    {0x010000, &g_Task0QueueDescriptor},
    {0x020000, &g_Task1QueueDescriptor},
    {0x090000, &g_Task2QueueDescriptor},
    {0x040000, &g_Task3QueueDescriptor},
    {0x050000, &g_Task4QueueDescriptor},
    {0x030000, &g_DisplayTaskQueueDescriptor},
    NULL
};





SyncObject g_Sync_UserScratchY;
_Y WORD g_wUserScratchY[SCRATCH_USER_Y_SIZE];

SyncObject g_Sync_UserScratchY;
_X WORD g_wUserScratchX[SCRATCH_USER_X_SIZE];

extern WORD g_wNumDrives;

void TimeInit(void);

//
// Stub functions needed because the libraries refer to the following functions.
//
void ChipUpdateCapability(void)
{
}

void EnterNonReentrantSection(void)
{
}

void LeaveNonReentrantSection(void)
{
}

// use this for tests that must halt instead of ver that resets the DSP in RETAIL build. 
#pragma asm
 global FatalError
FatalError:
    debug
    nop
#pragma endasm

_reentrant int Task0Region1Bank2(int a, int b, int *c);
//------------------------------------------------------------------------------

void _reentrant Project_Startup(void)
{
    int i;

    SysPostMessage(3,LCD_SET_CONTRAST,30);

    if(MediaInit(0) != SUCCESS)
    {
        SystemHalt();
    }

    // Discover the Media and its drives
    if(MediaDiscoverAllocation(0) != SUCCESS)
    {
        SystemHalt();
    }

    // Init All Drives
    for(i=0;i<g_wNumDrives;i++)
    {
        if(DriveInit(i) != SUCCESS)
        {
        }
    }

    SysSetResourceTag(DRIVE_TAG_RESOURCE_BIN);

    //this call shows that funclets/overlays work when called in Project_Startup.   
    SysCallFunction(RSRC_REGION1_TASK0_BANK2,Task0Region1Bank2,0,1,&i);

    TimeInit();
}