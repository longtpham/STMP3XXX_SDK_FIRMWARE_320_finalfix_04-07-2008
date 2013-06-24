////////////////////////////////////////////////////////////////////////////////
// Copyright(C) SigmaTel, Inc. 2004-2006
//
// File        : displaydirver_color.h
// Description : LCD driver Header file
////////////////////////////////////////////////////////////////////////////////
#ifndef _DISPLAY_DRIVER_H
#define _DISPLAY_DRIVER_H

//currently, framebuffer only works when located in the Y memory space.
#include "types.h"

_reentrant void HALDisplayInit(void);
_reentrant void HALDisplaySendCommand(WORD wCommand);
_reentrant void HALDisplaySendData(WORD wXAddressControl, WORD wYAddressControl, WORD wWidth, WORD wHeight, WORD* pBuffer, WORD wDisplayOperation);
_reentrant void HALDisplaySetContrast(INT iContrast);
_reentrant void HALDisplayDecContrast(void);
_reentrant void HALDisplayIncContrast(void);
_reentrant void HALHandleMessage(Message *pMsg);

_reentrant void HALDisplaySendPixelData(WORD wData, WORD wDisplayOperation);

#ifndef PLAYER_BUILD
_reentrant void HALDisplaySuspendIn(void);
_reentrant void HALDisplaySuspendOut(void);
#endif

#define LCD_START_ROW 0x01
#define LCD_START_COLUMN 0x02


#endif
