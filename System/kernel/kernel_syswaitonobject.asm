;//////////////////////////////////////////////////////////////////////////////
;/!
;/! \file syswaitonobject.asm
;/! \brief
;/! This implements the SysWaitOnObject kernel API.
;/!
;/! Copyright (c) SigmaTel, Inc. Unpublished
;/!
;/! SigmaTel, Inc.
;/! Proprietary  Confidential
;/!
;/! This source code and the algorithms implemented therein constitute
;/! confidential information and may comprise trade secrets of SigmaTel, Inc.
;/! or its associates, and any use thereof is subject to the terms and
;/! conditions of the Confidential Disclosure Agreement pursuant to which this
;/! source code was originally received.
;/!
;//////////////////////////////////////////////////////////////////////////////

    include 'kernel.inc'
    include "sysmacro.asm"
    include "const.xref"

    extern Kernel_ContextSwitch
    extern FKernel_SetTimeout
    extern Fg_pCurrentTask

    global  SysWaitOnObject

    org p,".ptextkernel_syswaitonobject":


;There might be some way to get the answer of one from the other.



;//////////////////////////////////////////////////////////////////////////////
;/ SysWaitOnObject
;/ _asmfunc WORD SysWaitOnObject(INT wTimeout, SyncObject *pObject);
;/-----------------------------------------------------------------------------                                               
;/                                             
;/ Inputs:
;/  a   - Timeout for the wait operation.
;/  r0  - Synchronization object to wait on.
;/
;/ Outputs:
;/
;/                                               
;//////////////////////////////////////////////////////////////////////////////
SysWaitOnObject
    push    ssh
    
    push    r0                                  ;save off the address of SyncObject
    jsr     FKernel_SetTimeout                  ;set the timeout length
    pop     r0                                  ;get another copy of the SyncObject address
    move    y:Fg_pCurrentTask,a
    move    y:(r0),b
    tst     a
    jeq     _already_own                        ;shortcut if called from the kernel (i.e. Fg_pCurrentTask == NULL)
    cmp     a,b
    jeq     _increment
    move    #>$ffff,x0
    and     x0,b
    cmp     a,b    
    jeq     _increment
    tst     b
    jeq     _nowait
    move    #>EVENT_OBJECT,a                    ;change return value to EVENT_OBJECT

    move    #>TASK_STATE_OFFSET,n1
    move    y:Fg_pCurrentTask,r1
    push    r0                                  ;save off the address of the sync object    
    move    a1,y:(r1+n1)
    move    #>1,n1
    nop
    move    r0,y:(r1+n1);
    jsr     Kernel_ContextSwitch                ;if the object is owned by somebody, we'll wait until its free or timed out.
    pop     r0                                  ;get the address of the SyncObject
_nowait
    clr     a                                   ;Lets assume EVENT_TIMEOUT as the return value
    move    y:(r0),b                            ;lets see if anybody owns it
    tst     b                                   
    jne     _exit                               ;if nobody owns it
    move    y:Fg_pCurrentTask,r1                ; take control
    move    r1,y:(r0)                           ;don't want to increment
_already_own
    move    #>EVENT_OBJECT,a                    ;change return value to EVENT_OBJECT
_exit
    pop ssh
    nop
    rts

_increment
    move    y:(r0),a                            ;increment the count by 1.
    move    y:<Const_010000,x0
    add     x0,a
    move    a1,y:(r0);
    jmp _already_own