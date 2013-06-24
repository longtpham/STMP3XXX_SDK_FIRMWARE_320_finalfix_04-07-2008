////////////////////////////////////////////////////////////////////////////////
// Copyright(C) SigmaTel, Inc. 2000-2001
//
// Filename: lradctest.h
// Description: 
////////////////////////////////////////////////////////////////////////////////

#ifndef _BUTTONTEST_H
#define _BUTTONTEST_H

#include "types.h"

#define NO_BUTTON	0xFFFFFF
#define NUM_KEYS_ON_KEY_PAD		12
#define NUM_REMOTE_KEYS			5
#define TWO_KEY_X				18
#define TWO_KEY_Y				50

extern int _reentrant DisplayModuleInit(struct ModuleEntry *pEntry);
extern struct ModuleEntry *ModuleTableDisplay;

extern _asmfunc void ButtonInit(void);
extern _asmfunc void ButtonsOn(void);
extern _asmfunc void ExecuteModules (void);
extern _asmfunc void SysDefaultAllVectors(void);
extern _asmfunc void SysResetAllInterrupts(void);
extern _asmfunc void SysResetAllTimers(void);
extern _asmfunc void SysTimeInit(void);

void PrintString(_packed BYTE* pString, int x, int y);
WORD _reentrant GetKey(void);


#endif // #ifndef _BUTTONTEST_H
