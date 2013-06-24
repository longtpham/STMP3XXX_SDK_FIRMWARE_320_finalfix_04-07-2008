


//This file overrides Kernel_ContextSwitch in kernel.mk and replaces it with the one that knows how to deal with overlays.

    include "sysmacro.asm"

    GLOBAL  Kernel_ContextSwitch

    EXTERN FKernel_pTempStack

    org p,".ptextkernel_contextswitch":

Kernel_ContextSwitch
    push    SR                       ;save the SR register
    push    SSH                      ;save the return address on the stack
    move    y:FKernel_pTempStack,r6  ;swap the stacks.
    move    r7,y:FKernel_pTempStack
    move    r6,r7
    nop 
    pop     SSH                     ;gets the PC that was pushed onto the software stack
    pop     SSL                     ;pop off the status register
    nop
    rti                             ;RTI instead of RTS in order to get the SR register.

