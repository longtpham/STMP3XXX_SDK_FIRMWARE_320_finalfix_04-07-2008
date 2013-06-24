////////////////////////////////////////////////////////////////////////////////
// Copyright(C) SigmaTel, Inc. 2000-2001
//
// Filename: lradctest.h
// Description: 
////////////////////////////////////////////////////////////////////////////////

#ifndef _I2CTEST_H
#define _I2CTEST_H

#include "types.h"

#define PLAY    4

extern int _reentrant DisplayModuleInit(struct ModuleEntry *pEntry);
extern struct ModuleEntry *ModuleTableDisplay;

extern _asmfunc void ButtonInit(void);
extern _asmfunc void ButtonsOn(void);
extern _asmfunc void SysResetAllInterrupts(void);
extern _asmfunc void SysResetAllTimers(void);
extern _asmfunc void SysTimeInit(void);
extern _asmfunc void SysAllLinear(void);

WORD _reentrant GetKey(void);

void I2C_Read_Master_Sync(int I2C_Device_Addr, int I2C_Sub_Addr, int num_words, /*_circ*/ int * ptr);
void I2C_Write_Master_Sync(int I2C_Device_Addr, int I2C_Sub_Addr, int num_words, /*_circ*/ int * ptr);

#endif // #ifndef _I2CTEST_H
