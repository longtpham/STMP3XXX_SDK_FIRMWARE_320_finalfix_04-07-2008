// Filename: icoll_invalidvector.c
// Copyright (c) SigmaTel, Inc. 2001-2005
//
#include "types.h"

// Description: Plan is to only call this in a DEBUG build as the default ISR. 
//              RETAIL builds call FatalError to reset instead of this.
//              
_reentrant void IcollInvalidVector(void)
{
#if 1
    assert(0); // warning: this resets the SOC in RETAIL builds (assert->error->FatalError|debug). 
    // Verified for MTP re-enumation. Halts with debug instr in DEBUG builds.
    // Instead of reset, we could just do nothing and return in RETAIL builds, 
    // but ISR may be high frequency so we chose to reset. 
#else
    // halts in DEBUG builds as did previous version.
    DebugBuildAssert(0); // vanishes to do nothing and return in RETAIL builds. (ignoring unassigned interrupt & returning to foreground)
    __asm(" nop ");
    __asm(" rti ");  // return from interrupt. 
    // We need this rti to return in RETAIL case if this handler is not registered with the compiler as an ISR.
    // If you change the SDK to register this function as an ISR, then comment out the rti above.
    // Note: It may be fastest to have the vector table blank for unused interrupts in the RETAIL case 
    // so a handler won't even be called. In moto speak, that's a "fast interrupt" & the ProgCtr returns immediately from the vector table.
    // But we defaulted the entire table to jsr to this function or to FatalError in RETAIL builds.
#endif
}
    //If you've halted here in a DEBUG build, it means an interrupt was triggered that didn't have a vector associated with it.  
    //It could also be because something has gone off in the weeds and jumped to 0 or some low address.  
    //Look back in the OnCE 5 word Program Counter tracebuffer to see where you came from.


