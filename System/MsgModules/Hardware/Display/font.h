#ifndef __FONT_H
#define __FONT_H

_reentrant INT GetTextWidthAddressUnicode(WORD *pUnicodeString);
_reentrant INT GetTextWidthResourceUnicode(WORD wResource);
_reentrant INT GetTextWidthAddressDBCS(_packed unsigned char *pDBCSString);
_reentrant INT GetTextWidthResourceDBCS(WORD wResource);

#endif
