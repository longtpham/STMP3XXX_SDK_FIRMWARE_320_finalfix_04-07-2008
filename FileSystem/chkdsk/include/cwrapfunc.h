////////////////////////////////////////////////////////////////////////////////
// Copyright(C) SigmaTel, Inc. 2000-2004
//
// Filename: cwrapfunc.h
// Description: 
////////////////////////////////////////////////////////////////////////////////

#ifndef _CHKDSK_CWRAPFUNC_H
#define _CHKDSK_CWRAPFUNC_H

#include "types.h"

_asmfunc void SetupSwizzleAddrXY(WORD _X *pwSource, WORD _Y *pwDest);
_asmfunc void SetupSwizzleAddrYX(WORD _Y *pwSource, WORD _X *pwDest);

#endif 
