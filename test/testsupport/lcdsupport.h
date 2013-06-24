////////////////////////////////////////////////////////////////////////////////
// Copyright(C) SigmaTel, Inc. 2000-2001
//
// Filename: lcdsupport.h
// Description: LCD Support functions
////////////////////////////////////////////////////////////////////////////////

#ifndef _LCDSUPPORT_H
#define _LCDSUPPORT_H

#include "types.h"

void _reentrant SysPostMessage(int iLength,...);

// File System defines
#define RD_ONLY_FUNC    0
#define RW_FUNC         1

// Error
#define NO_ERROR        0
#define ERROR           -1

// LCD defines
#define LCD_X_PIXELS    98
#define LCD_Y_PIXELS    64

void _reentrant RetrieveLcdMessage(void);
void _reentrant Delay(WORD wDelay);
void _reentrant SwizzleText(_packed BYTE *pText);

void _reentrant LcdClear(WORD wPosX, WORD wPosY, WORD wLength, WORD wHeight);
void _reentrant PrintRangeRsrc(WORD wPosX, WORD wPosY, WORD wRsrc);
void _reentrant PrintRangeAddr(WORD wPosX, WORD wPosY, WORD *pData);
void _reentrant PrintRangeInvRsrc(WORD wPosX, WORD wPosY, WORD wRsrc);
void _reentrant PrintRangeInvAddr(WORD wPosX, WORD wPosY, WORD *pData);
void _reentrant PrintStringRsrc(WORD wPosX, WORD wPosY, WORD wRsrc);
void _reentrant PrintStringAddr(WORD wPosX, WORD wPosY, _packed BYTE *pMsgString);
void _reentrant ScrollDisplay(INT wLRdist, INT wUDdist);
void _reentrant SetContrast(WORD wContrasVal);
void _reentrant IncContrast(void);
void _reentrant DecContrast(void);
void _reentrant SetFont(WORD wFontIndex);
void _reentrant PrintNumber(WORD wMsgXPos, WORD wMsgYPos, WORD wNum, WORD wWidth, WORD wLeadChar);
void _reentrant PrintTime(WORD wMsgXPos, WORD wMsgYPos, WORD wMinutes, WORD wSeconds);

#endif // #ifndef _LCDSUPPORT_H
