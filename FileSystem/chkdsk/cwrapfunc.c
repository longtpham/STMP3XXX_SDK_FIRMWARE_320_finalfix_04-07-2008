////////////////////////////////////////////////////////////////////////////////
// Copyright(C) SigmaTel, Inc. 2000-2004
//
// Filename: cwrapfunc.c
// Description: C wrappers for asm functions
////////////////////////////////////////////////////////////////////////////////

#include "cwrapfunc.h"

#pragma asm
    org p,".ptextcwrapfunc":

    SetupSwizzleAddrYX:
    SetupSwizzleAddrXY:
        move    r0,x:$f383              ; Source Address
        move    r4,x:$f386              ; Destination Address
        rts            
#pragma endasm        
