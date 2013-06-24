////////////////////////////////////////////////////////////////////////////////
// Copyright(C) SigmaTel, Inc. 2000-2001
//
// Filename: fmtunertest.h
// Description: Tests to verify the functionality of the FMTuner
////////////////////////////////////////////////////////////////////////////////

#ifndef _FMTUNERTEST_H
#define _FMTUNERTEST_H

#include "types.h"
#include "..\..\..\system\msgmodules\hardware\tuner\tunerdriver.h"
extern _asmfunc void ButtonsOn(void);

extern _asmfunc void ButtonInit(void);
extern _asmfunc void SysResetAllInterrupts(void);
extern _asmfunc void SysResetAllTimers(void);
extern _asmfunc void SysTimeInit(void);
extern _asmfunc void ExecuteModules(void);

extern int _reentrant TunerModuleInit(struct ModuleEntry *);
extern int _reentrant TunerModuleProcessQueue(union WaitMask ,struct MessageQueueDescriptor *);
extern int _reentrant DisplayModuleInit(struct ModuleEntry *pEntry);
extern struct ModuleEntry *ModuleTableDisplay;

extern struct ModuleEntry			 *TunerModuleEntry;
extern struct MessageQueueDescriptor *glb_TunerQueueDescriptor;
extern _reentrant INT TunerDriverTuneToFrequency(INT); 

_reentrant void DisplayFrequency(void);
_reentrant void FMTAnalogInit(void);
_reentrant void ForceFMTInit (void);

#endif // #ifndef _FMTUNERTEST_H
