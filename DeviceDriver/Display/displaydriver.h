#ifndef _DISPLAY_DRIVER_H
#define _DISPLAY_DRIVER_H

//currently, framebuffer only works when located in the Y memory space.
#include "types.h"

_reentrant void HALDisplayInit(void);
_reentrant void HALDisplaySendCommand(WORD wCommand);
_reentrant void HALDisplaySendData(WORD wPage, WORD wColumn, WORD wCount, WORD wByteIndex,WORD* pBuffer, WORD wDisplayOperation);
_reentrant void HALDisplaySetContrast(INT iContrast);
_reentrant void HALDisplayDecContrast(void);
_reentrant void HALDisplayIncContrast(void);
_reentrant void HALHandleMessage(Message *pMsg);




#endif
