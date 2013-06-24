

    include "sysmacro.asm"
    include "kernel.inc"
    include "const.xref"

    GLOBAL  SysCallFunction
    GLOBAL  v_SysCallFunction_v
    GLOBAL  SysCallFunction_i

    org p,".ptextkernel_syscallfunction":

    ;r0 is the resource
    ;r4 has the address of the function required

SysCallFunction
v_SysCallFunction_v
SysCallFunction_i
    push SSH
    ;save off the function to call
    push    r4
    ;re-order the parameters
    move r1,r0
    move r5,r4
    move r2,r1
    move r6,r5
    move r3,r2

    pop     r6
    nop 
    jsr     (r6)

    pop     SSH
    nop
    rts