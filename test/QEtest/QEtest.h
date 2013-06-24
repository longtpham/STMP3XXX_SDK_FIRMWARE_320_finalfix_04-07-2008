////////////////////////////////////////////////////////////////////////////////
// Copyright(C) SigmaTel, Inc. 2000-2001
//
// Filename: QEtest.h
// Description: 
////////////////////////////////////////////////////////////////////////////////

#ifndef _QETEST_H
#define _QETEST_H

#include "types.h"

extern int _reentrant DisplayModuleInit(struct ModuleEntry *pEntry);
extern struct ModuleEntry *ModuleTableDisplay;

extern _asmfunc void ButtonInit(void);
extern _asmfunc void ButtonsOn(void);
extern _asmfunc void ExecuteModules (void);
extern _asmfunc void SysResetAllInterrupts(void);
extern _asmfunc void SysResetAllTimers(void);
extern _asmfunc void SysTimeInit(void);

void MultipleKeyPress(void);
void PrintEventString(void);
void PrintEncoderString(void);
WORD _reentrant GetKey(void);


#endif // #ifndef _QETEST_H
