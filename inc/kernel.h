///////////////////////////////////////////////////////////////////////////////
///
/// Filename: kernel.h
/// Description: 
///
///
/// Copyright (c) SigmaTel, Inc. Unpublished
///
/// SigmaTel, Inc.
/// Proprietary  Confidential
///
/// This source code and the algorithms implemented therein constitute
/// confidential information and may comprise trade secrets of SigmaTel, Inc.
/// or its associates, and any use thereof is subject to the terms and
/// conditions of the Confidential Disclosure Agreement pursuant to which this
/// source code was originally received.
///
///////////////////////////////////////////////////////////////////////////////
#ifndef __KERNEL_H
#define __KERNEL_H


#include "types.h"
#include "gettime.h"
#include "sysirq.h"
#include "icoll.h"

#ifndef SYS_MAX_MSG_PARAMS
#define SYS_MAX_MSG_PARAMS 10
#endif

#ifndef MAX_REGIONS
#define MAX_REGIONS 5
#endif 

#define TIMEOUT_IMMEDIATE 0

struct TaskEntryStruct;

union SyncObjectStruct;


// JCP - Already defined in types.h

//typedef struct {
//    WORD m_wLength;
//    WORD m_wMessage;
//    WORD m_wParams[SYS_MAX_MSG_PARAMS];
//} Message;

typedef struct {
    _circ WORD    *m_pHead;
    _circ WORD    *m_pTail;
} CircularBufferDescriptor;

typedef struct MemoryRegionStruct {
    WORD m_wResourceLoaded;
    _P INT *m_StartAddress;
    _P INT *m_EndAddress;
} MemoryRegion;


typedef struct TaskEntryStruct{
    CircularBufferDescriptor *m_pMessageQueue;
    void   *m_pWaitingOnPtr;                                //points to either an object or a queue we're waiting on.
    WORD    m_wWaitingOnWord;
    WORD    m_wState;                                       //The state is used to indicate what the task is waiting on.
    WORD   *m_pStackPointer;
    WORD    m_wTaskID;                                      //The kernel does not use this for any purpose.   This is set by the developer
                                                            //  and returned by SysGetTaskID() for use by functions that are required to 
                                                            //  be task aware.
    long    m_lTimeout;                                     //This is in absolute time (can compare directly to the current system time
    WORD    m_wRegionResources[MAX_REGIONS];                //a snapshot of the requirements of the task in terms of which resources are loaded
    WORD    m_wPriority;                                    
    WORD   *m_pInitialStackPointer;                         //only used at init time
    WORD    m_wStackSize;
    _reentrant void   (*m_pEntryPoint)(void);               //only used at init time
} TaskEntry;

typedef union SyncObjectStruct {
    TaskEntry   *m_pOwner;
    struct
    {
    int          m_iOwner :16;
    int          m_iCount : 8;
    } counter;
} SyncObject;

typedef WORD SignalGroup;


typedef struct {
    WORD    m_wMessageType;
    CircularBufferDescriptor    *m_pDestination;
}  RoutingEntry;

extern RoutingEntry g_RoutingTable[];
extern TaskEntry *g_TaskList[];
extern MemoryRegion *g_RegionTable[];

//Time Releated Functions
_reentrant WORD SysWait(INT wTimeout);

//Task Related Functions
_reentrant WORD SysGetTaskID(void);

//Message related functions
_asmfunc WORD SysWaitOnMessage(INT wTimeout, Message *pMsg);
_asmfunc WORD SysPeekMessage(CircularBufferDescriptor *pQueue, Message *pMsg);
_reentrant WORD SysPostMessage(WORD wLength, ...);
_reentrant WORD SysPostMessageWait(INT wTimeout, WORD wLength, ...);

//maybe this is too much functionality?
_asmfunc WORD SysWaitOnBoth(WORD wTimeout, SyncObject *pObject, Message *pMsg);

//SyncObject Related functions
_asmfunc WORD SysWaitOnObject(INT wTimeout, SyncObject *pObject);
_asmfunc WORD SysAssignObject(SyncObject *pObject, TaskEntry *pTaskID);
_reentrant void SysReleaseObject(SyncObject *pObject);
_reentrant void ISRReleaseObject(SyncObject *pObject);

//Queue Related Functions
_asmfunc WORD SysWaitOnQueue(INT wTimeout,CircularBufferDescriptor *pQueue, INT wSizeToBeFree);

//Event Related Functions
_asmfunc WORD SysWaitOnSignal(INT wTimeout, void * pSignalGroup, int iSignal);
_asmfunc void SysSignal(SignalGroup *pSignalGroup, int iSignal);

//ISR function

//SysCallFunctions
int _asmfunc exSysCallFunction_v(void);
int _asmfunc exSysCallFunction_i(int);
int _asmfunc exSysCallFunction_ii(int, int);
int _asmfunc exSysCallFunction_iii(int, int, int);
int _asmfunc SysCallFunction(WORD * RESOURCE,int _reentrant (int,int,int*),  int, int, int *);
#define SysCallFunction(rsrc,func,a,b,p) SysCallFunction((void*)rsrc,(func),(a),(b),(p))

void _asmfunc v_SysCallFunction_v(WORD * RESOURCE,void _reentrant (void));
#define v_SysCallFunction_v(rsrc,func) v_SysCallFunction_v((void*)rsrc,(func))

void _asmfunc SysCallFunction_i(WORD * RESOURCE,void _reentrant (int),int);
#define SysCallFunction_i(rsrc,func,a,b,p) SysCallFunction_i((void*)rsrc,(func),(a),(b),(p))

int _asmfunc SysCallFunction_L(void* RESOURCE,int _reentrant (long,long,int*),  long, long, int *);

int _asmfunc i_SysCallFunction_i(WORD * RESOURCE,int _reentrant (int), int);

int _asmfunc i_SysCallFunction_iipp(WORD * RESOURCE,int _reentrant (int,int,int*,int*),  int, int, int *, int *);

int _asmfunc i_SysCallFunction_iiip(WORD * RESOURCE,int _reentrant (int,int,int,int*),  int, int, int, int *);

int _asmfunc fSysCallFunction(WORD * RESOURCE,int _reentrant (int,int,int*),  int, int, int *);


//These values are return values, and also values that are set in the TaskEntry.m_wState to indicate what the task
//is waiting on.
#define EVENT_TIMEOUT   0
#define EVENT_MESSAGE   1
#define EVENT_OBJECT    2
#define EVENT_QUEUE     4
#define EVENT_SIGNAL    5
#define EVENT_RELEASING_OBJECT2 6
#define EVENT_RELEASING_OBJECT1 7


//this is the owner that a SyncObject should be assigned to if a task it going to wait on a completion of the interrupt. 
#define ISR_OWNER  ((TaskEntry*)(0xffff))
#define KERNEL_OWNER  ((TaskEntry*)(0))


#ifdef INSTRUMENT_ENABLE
#include <regsgpio.h>

//
// Define the bit positions in the GPIO registers used for signalling.
//
#define GPIO_24     B0
#define GPIO_36     B12
#define GPIO_37     B13
#define GPIO_38     B14
#define GPIO_39     B15
#define GPIO_42     B18
#define GPIO_46     B22
#define GPIO_47     B23

//=============================================================================
// SysSignalEvent
//-----------------------------------------------------------------------------
//! This macro allows code to set a gpio event that can be measured outside the
//! part.
//! \param wGPIONumber 
//!     This should be one of the Macros defined above, and indicates which 
//!     GPIO will be set.
//!
//! \return
//!     None. 
//!
//=============================================================================
#define SysSignalEvent(wGPIONumber)         \
{                                           \
    HW_GP1DOR.B.##wGPIONumber = 1;          \
}

//=============================================================================
// SysClearEvent
//-----------------------------------------------------------------------------
//! This macro allows code to clear a gpio event that can be measured outside 
//! the part.
//! \param wGPIONumber 
//!     This should be one of the Macros defined above, and indicates which 
//!     GPIO will be set.
//!
//! \return
//!     None. 
//!
//=============================================================================
#define SysClearEvent(wGPIONumber)          \
{                                           \
    HW_GP1DOR.B.##wGPIONumber = 0;          \
}

//=============================================================================
// SysEnableEvent
//-----------------------------------------------------------------------------
//! This macro must be called by code before using the SysSetEvent or 
//! SysClearEvent so that the GPIOs are set up correctly.
//!
//! \param wGPIONumber 
//!     This should be one of the Macros defined above, and indicates which 
//!     GPIO will be set.
//!
//! \return
//!     None. 
//!
//=============================================================================
#define SysEnableEvent(wGPIONumber)         \
{                                           \
    HW_GP1DOER.B.##wGPIONumber=1;           \
    HW_GP1DOR.B.##wGPIONumber=0;            \
    HW_GP1ENR.B.##wGPIONumber=1;            \
    HW_GP1PWR.B.##wGPIONumber=1;            \
}
#else // INSTRUMENT_ENABLE
#define SysSignalEvent(wGPIONumber)
#define SysClearEvent(wGPIONumber)
#define SysEnableEvent(wGPIONumber)
#endif // INSTRUMENT_ENABLE
    
#endif