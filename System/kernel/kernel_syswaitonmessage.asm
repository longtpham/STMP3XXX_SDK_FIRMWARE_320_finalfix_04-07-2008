;//////////////////////////////////////////////////////////////////////////////
;/ Filename: kernel_syswaitonmessage.asm
;/                                               
;/ Description: This file implements the SysWaitOnMessage API.
;/
;/ Copyright (c) SigmaTel, Inc. Unpublished
;/
;/ SigmaTel, Inc.
;/ Proprietary  Confidential
;/
;/ This source code and the algorithms implemented therein constitute
;/ confidential information and may comprise trade secrets of SigmaTel, Inc.
;/ or its associates, and any use thereof is subject to the terms and
;/ conditions of the Confidential Disclosure Agreement pursuant to which this
;/ source code was originally received.
;/                                               
;//////////////////////////////////////////////////////////////////////////////

    include 'kernel.inc'
    include "sysmacro.asm"
    include "const.xref"

    extern Kernel_ContextSwitch
    extern FKernel_SetTimeout
    extern Fg_pCurrentTask
    extern QueueGetFilledSpace
    extern QueueGetEmptySpace

    global  Kernel_IsMessageAvailable

    org p,".ptextkernel_syswaitonmessage":

SysGetMessage
    push    r0                          ;Save off the message destination buffer address
    move    y:Fg_pCurrentTask,r0        ;
    push    SSH
    jsr     Kernel_IsMessageAvailable   ;get the amount of filled space
    pop     SSH
    pop     r4                          ;put our 
    clr     b
    tst     a
    jeq     _exit
    move    y:(r0)+                     ;advance past the head pointer
    move    y:(r0)+,m2                  ;get the modulo
    move    a,b0
    clr     a y:(R0),r2                ;get the tail pointer
    nop
    move    y:(r2)+,a0
    tst     a                           ;make sure there's some length
    ccerror eq                          ;if we're stopping here, then there's no message length (that should never happen)
    cmp     a,b                         ;make sure the filled length is greater than or equal to the message length
    ccerror lt                      
    move    a0,y:(r4)+                   ;copy the length
    dec     a                           ;take one off the length
    do  a0,_enddo                       ; copy the rest
        move    y:(r2)+,a
        move    a1,y:(r4)+
_enddo
    move    r2,y:(r0)                   ;copy back the tail pointer.
    move    y:<Const_ffffff,m2          ;restore the modulo register
    move    #>EVENT_MESSAGE,a
_exit                    
    rts


Kernel_IsMessageAvailable
    ;r0 is the TaskEntry
    ;a is returned non zero if a message is waiting
    move    y:(r0),r0
    push    SSH
    jsr     QueueGetFilledSpace
    pop     SSH
    nop
    rts    

;//////////////////////////////////////////////////////////////////////////////
;/ SysWaitOnMessage
;/-----------------------------------------------------------------------------
;/ _asmfunc WORD SysWaitOnMessage(INT wTimeout, Message *pMsg);
;/                                               
;/ Inputs:
;/  a   - Timeout for this call.
;/  r0  - Pointer to return the message in once it arrives.
;/
;/ Outputs:
;/
;//////////////////////////////////////////////////////////////////////////////
SysWaitOnMessage
    push    ssh
    push    r0                                  ;save off the address of where to put the message
    jsr     FKernel_SetTimeout                  ;set the timeout length
    move    y:Fg_pCurrentTask,r0                
    ;
    ; Check if we are in the kernel or not.
    ; Assumes KERNEL_OWNER is 0.
    tst     a
    if (@def('DEBUG'))
        ;
        ; In debug mode cause a break
        ;
        jne     _NotInKernel
        debug
    else
        ;
        ; In retail mode just return
        ;
        jeq     _exit
    endif
_NotInKernel    
    jsr     Kernel_IsMessageAvailable           ;see if a message is available
    jne     _nowait
    move    #>EVENT_MESSAGE,a
    move    y:Fg_pCurrentTask,r0                
    move    #>TASK_STATE_OFFSET,n0
    nop
    move    a1,y:(r0+n0)
    jsr     Kernel_ContextSwitch                ;if there's no message, we'll wait until the message appears or we timeout
_nowait
    pop     r0
    jsr     SysGetMessage                       ;SysGetMessage will return 0 (which corresponds to EVENT_TIMEOUT) if no message waiting
    jne     _exit
    move    #>EVENT_MESSAGE,a
_exit
    pop ssh
    nop
    rts