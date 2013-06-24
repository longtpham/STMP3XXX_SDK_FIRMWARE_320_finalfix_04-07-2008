////////////////////////////////////////////////////////////////////////////////
// Copyright(C) SigmaTel, Inc. 2004-2006
//
// File        : display_sal_color.h
// Description : LCD driver Header file
////////////////////////////////////////////////////////////////////////////////
#ifndef __DISPLAY_SAL_H
#define __DISPLAY_SAL_H

_reentrant void SALDisplayClearRange(INT x,INT y,WORD iSourceWidth,WORD iSourceHeight, INT iFrameBufferID);
_reentrant void SALDisplayPrintBitmapAddress   (INT x,INT y,void _Y *pAddress ,INT bInvert, INT iFrameBufferID);
_reentrant void SALHandleMessage(Message  *pMsg);
_reentrant void SALDisplayUpdate(void);
_reentrant void SALDisplayInit(void );

_reentrant INT SALDisplayGetWidth(void);
_reentrant INT SALDisplayGetHeight(void);
_reentrant INT SALGetBufferWidth(INT iFrameBufferID);

#endif