
    include 'kernel.inc'
    include "sysmacro.asm"
    include "const.xref"

    extern Kernel_ContextSwitch
    extern FKernel_SetTimeout
    extern Fg_pCurrentTask
    extern SysGetQueueEmptySpace

    global  SysWaitOnQueue

    org p,".ptextkernel_syswaitonqueue":






SysWaitOnQueue
    push    ssh
    push    b1
    push    r0                                  ;save off the address of the Queue
    jsr     FKernel_SetTimeout                  ;set the timeout length
    pop     r0                                  ;get another copy of the Queue address
    push    r0
    jsr     SysGetQueueEmptySpace
    pop     r0
    pop     b
    cmp     a,b     
    move    #>EVENT_QUEUE,a
    move    y:Fg_pCurrentTask,r1
    jgt     _exit    
    move    y:(r1)+
    move    r0,y:(r1)+                          ;store the address (m_pWaitingOnPtr) into the structure
    move    b1,y:(r1)+                          ;store the minimum value (m_wWaitingOnWord) into the structure
    move    a1,y:(r1)+                          ;store the m_wState (EVENT_QUEUE) into the structure
    push    b1
    push    r0
    jsr     Kernel_ContextSwitch                ;we'll wait space is available or until timed out.
    pop     r0
    jsr     SysGetQueueEmptySpace               ;see if we've got enough room
    pop     b
    cmp     a,b
    move    #>EVENT_QUEUE,a
    jgt     _exit
    clr     a                                   ;this puts EVENT_TIMEOUT into A (same as 0)
_exit
    pop ssh
    nop
    rts