////////////////////////////////////////////////////////////////////////////////
// Copyright(C) SigmaTel, Inc. 2000-2003
//
// Filename: Fattest.h
// Description: 
////////////////////////////////////////////////////////////////////////////////

#ifndef _FATTEST_H
#define _FATTEST_H

extern _asmfunc RETCODE InitSmartMediaSense(WORD wNum);
extern _asmfunc BOOL SmIsDeviceInserted(WORD wNum);

extern int _reentrant DisplayModuleInit(struct ModuleEntry *pEntry);
extern struct ModuleEntry *ModuleTableDisplay;

#endif // #ifndef _FATTEST_H
