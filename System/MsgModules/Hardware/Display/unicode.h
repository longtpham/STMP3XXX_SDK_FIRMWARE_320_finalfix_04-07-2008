#ifndef UNICODE_H
#define UNICODE_H

//#include "displaydrv.h"

// Exported functions
_reentrant BITMAP *  GetUnicodeCharacterBitmap(WORD c) ;

void _reentrant CLcdPrintStringAddress(WORD x, WORD y, WORD *string, WORD bInverted);
#ifndef USBMSC_BUILD
void _reentrant LCDSetFont(WORD fontNum);
#else
void LCDSetFont(WORD fontNum);
#endif

#endif