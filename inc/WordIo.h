////////////////////////////////////////////////////////////////////////////////
// Copyright(C) SigmaTel, Inc. 2000-2001
//
// Filename: WordIo.h
// Description: 
////////////////////////////////////////////////////////////////////////////////

#ifndef _WORDIO_H
#define _WORDIO_H


WORD _reentrant GetWordX(WORD wByteNumber, WORD _X *pwBuffer);
DWORD _reentrant GetDWordX(WORD wByteNumber, WORD _X *pwBuffer);
WORD _reentrant GetWordY(WORD wByteNumber, WORD _Y *pwBuffer);
void _reentrant SetWordY(WORD wByteNumber, WORD wWord, WORD _Y *pwBuffer);

#endif 
