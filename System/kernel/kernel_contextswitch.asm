

    include "sysmacro.asm"

    GLOBAL  Kernel_ContextSwitch

    EXTERN FKernel_pTempStack

    org p,".ptextkernel_contextswitch":


Kernel_ContextSwitch
    push    sr                       ;save the SR register
    push    SSH                      ;save the return address on the stack
    move    sp,a
    tst     a
    jeq     _done_saving
_keepsaving
    push    SSL
    push    SSH
    move    sp,b                
    tst     b
    jne    _keepsaving
_done_saving
    push    a
    move    y:FKernel_pTempStack,r6  ;swap the stacks.
    move    r7,y:FKernel_pTempStack
    move    r6,r7
    nop
    pop     a
    tst     a
    jeq     _done_restoring
_keeprestoring
    pop     SSH
    pop     SSL
    move    sp,x0
    cmp     x0,a
    jne _keeprestoring
_done_restoring    
    pop     SSH                     ;gets the PC that was pushed onto the software stack
    pop     sr                      ;pop off the status register
    rts 
