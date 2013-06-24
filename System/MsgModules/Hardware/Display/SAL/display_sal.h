#ifndef __DISPLAY_SAL_H
#define __DISPLAY_SAL_H

_reentrant void SALDisplayClearRange(INT x,INT y,WORD iSourceWidth,WORD iSourceHeight);
_reentrant void SALDisplayPrintBitmapAddress   (INT x,INT y,void _Y *pAddress ,INT bInvert);
_reentrant void SALHandleMessage(Message  *pMsg);
_reentrant void SALDisplayUpdate(void);
_reentrant void SALDisplayInit(void );

_reentrant INT SALDisplayGetWidth(void);
_reentrant INT SALDisplayGetHeight(void);


#endif