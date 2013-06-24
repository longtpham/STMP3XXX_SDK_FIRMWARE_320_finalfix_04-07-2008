////////////////////////////////////////////////////////////////////////////////
// Copyright(C) SigmaTel, Inc. 2000-2001
//
// Filename: types.h
// Description: 
////////////////////////////////////////////////////////////////////////////////

#ifndef _SYSGPIO_H
#define _SYSGPIO_H

//RETCODE SysGpioPadPowerUp(WORD wGpioNum);
RETCODE SysGpioEnableAsInput(WORD wGpioNum);
RETCODE SysGpioEnableAsOutput(WORD wGpioNum);
RETCODE SysGpioDisable(WORD wGpioNum);
WORD SysGpioGetInputState(WORD wGpioNum);
RETCODE SysGpioSetOutputState(WORD wGpioNum, WORD wState);

#endif // #ifndef _SYSGPIO_H
