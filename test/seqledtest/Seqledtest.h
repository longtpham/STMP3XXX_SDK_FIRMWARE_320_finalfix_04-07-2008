////////////////////////////////////////////////////////////////////////////////
// Copyright(C) SigmaTel, Inc. 2000-2001
//
// Filename: lradctest.h
// Description: 
////////////////////////////////////////////////////////////////////////////////

#ifndef _LEDTEST_H
#define _LEDTEST_H


/////////////////////////////////////////////////////////////////////////////////
//   Includes
/////////////////////////////////////////////////////////////////////////////////
//	#include "exec.h"		   //prototype for SysPostMessage in here
#include "types.h"
#include "messages.h"
#include "hwequ.h"

void _reentrant SysPostMessage(int iLength,...);
	
/////////////////////////////////////////////////////////////////////////////////
//   Defines
/////////////////////////////////////////////////////////////////////////////////

// Offsets within a module table entry
#define Module_EventSignal      0
#define Module_EventWait        1
#define Module_ResourceNumber   2
#define Module_QueueDescriptor  3
#define Module_ProcessEvent     4
#define Module_Init             5
#define Module_Timer_High       6
#define Module_Timer_Low        7
#define Module_Background       8

extern _asmfunc SysResetAllTimers(void);
extern _asmfunc SysTimeInit(void);

#endif // #ifndef _LEDTEST_H
