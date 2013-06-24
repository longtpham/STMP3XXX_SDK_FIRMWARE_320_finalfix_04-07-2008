;//////////////////////////////////////////////////////////////////////////////
;/                                               
;/ Filename:kernel_sysassignobject.asm
;/                                               
;/ Description:   
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

    include 'const.xref'    
    include "sysmacro.asm"

    extern FSysMaskAllInterrupts
    extern FSysUnMaskAllInterrupts
    extern Fg_pCurrentTask

    global  SysAssignObject

    org p,".ptextkernel_sysassignobject":

;//////////////////////////////////////////////////////////////////////////////
;/ SysAssignObject
;/-----------------------------------------------------------------------------                                               
;/ SysAssignObject(SyncObject *pObject, TaskEntry *pTaskID)
;/                                               
;/ Description:   
;/
;/ Inputs:
;/  r0 - Pointer to a SyncObject.
;/  r4 - Pointer to the task that wants to own this object.
;/
;/ Outputs:
;/
;//////////////////////////////////////////////////////////////////////////////
SysAssignObject
    push    r0
    push    r4
    jsr     FSysMaskAllInterrupts
    pop     r4
    pop     r0
    push    a
    move    y:(r0),a              ;see if the old address was zero/NULL
    tst     a
    jeq     _ok                 ;if so, its ok to re-assign
    move    r4,b                ;see if the new address is the same as the old address
                                ;the count field of the syncobject must be 0!
    cmp     a,b             
    jeq     _ok                 ;if so, its ok to re-assign 
    move    y:Fg_pCurrentTask,b          ;see if the current task owns the object
    ; 
    ; If zero then this has been called from kernel which is an error
    ; Assumes KERNEL_OWNER is 0.
    tst     b
    if (@def('DEBUG'))
        jne     _testvalue
        debug
    else
        jeq     _exit
    endif    
_testvalue    
    cmp     a,b
    jne     _exit               ;if not, exit (and A will be !SUCCESS)
_ok
    clr a   r4,y:(r0)
_exit
    tfr     a,b
    pop     a
    jsr     FSysUnMaskAllInterrupts
    tfr     b,a
    rts
