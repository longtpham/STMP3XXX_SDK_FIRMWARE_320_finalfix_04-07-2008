
    include 'kernel.inc'
    include "sysmacro.asm"
    include "const.xref"

    extern Kernel_ContextSwitch
    extern FKernel_SetTimeout
    extern Fg_pCurrentTask

    global  SysWaitOnSignal

    org p,".ptextkernel_syswaitonsignal":

SysWaitOnSignal
    ;R0:  the SignalGroup pointer
    ;A1:  the timeout, in milliseconds to wait
    ;B1:  The signal in the group to wait on.
    push    ssh
    push    b1
    push    r0                                  ;save off the address of the SignalGroup
    jsr     FKernel_SetTimeout                  ;set the timeout length
    pop     r0                                  ;pop off the SignalGroup
    pop     a                                   ;pop off the signal number
    push    a1                                  ;save off the signal number
    push    r0                                  ;save off the signalgroup again
    jsr     kernel_IsSignalSet                  ;see if its signaled    
    tst     a                                   ;A is non-zero is its set
    move    #>EVENT_SIGNAL,a                    ;assume it is signaled
    pop     r0                                  ;load up the signal group
    pop     b1                                  ;and the signal
    move    y:Fg_pCurrentTask,r1                ;put a pointer to the current TaskEntry into r1
    jne     _exitclearingsignal                 ;
_notSet
    move    y:(r1)+                             ;increment past the message queue
    move    r0,y:(r1)+                          ;store the address of the signal group (m_pWaitingOnPtr) into the structure
    move    b1,y:(r1)+                          ;store the signal number (m_wWaitingOnWord) into the structure
    move    a1,y:(r1)+                          ;store the m_wState (EVENT_SIGNAL) into the structure
    push    b1
    push    r0
    jsr     Kernel_ContextSwitch                ;we'll wait until the signal is set, or until timed out.
    pop     r0
    pop     a1
    push    a1                                  ;save off the Signal number
    push    r0                                  ;save off the SignalGroup address
    jsr     kernel_IsSignalSet                  ;see if the signal is set
    pop     r0                                  ;pop out the signal group
    pop     b                                   ;pop out the signal number
    tst     a                                   
    clr     a                                   ;set the signal to EVENT_TIMEOUT
    jne     _exit                               ;if the signal is not set, exit with EVENT_TIMEOUT
_exitclearingsignal    
    move    b1,x0
    move    y:<Const_000001,y0
    mpy     y0,x0,a y:(r0),b
    move    a0,a
    not     a
    move    a1,x0
    and     x0,b
    move    b1,y:(r0)
    move    #>EVENT_SIGNAL,a
_exit
    pop ssh
    nop
    rts
    
kernel_IsSignalSet
    ;r0 is the pointer to the signal group
    ;a1 has the signal number to look at
    move    a1,y0
    move    y:<Const_000001,x0
    mpy     x0,y0,a
    move    y:(r0),x0
    move    a0,a
    and     x0,a
    rts
    
    