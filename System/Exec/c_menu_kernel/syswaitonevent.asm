    section SYSWAITONEVENT
    
    include "msgequ.inc"
    include "sysmacro.asm"
    include "const.xref"


;Functions
    extern FUserTask
    extern SysGetButton
    extern SysGetCurrentTime
    extern ContextSwitch
;Variables in Y
    extern MenuWaitingMsg
    extern MenuTimerHigh
    extern MenuTimerLow
    extern MenuQueueDescriptor
    extern MenuEventWaitMask
    extern GetMessage
    extern MenuIntervalHigh
    extern MenuIntervalLow
    GLOBAL SysWaitOnEvent
    extern EVENT_TIMER 

    org p,"MenuManager_p":

ReturnFromWait
    ;A1 is the event trigger mask
    ;R0 points to the message queue descriptor
    move y:MenuEventWaitMask,x0               ;put our wait mask 
    ;and it with the trigger mask 
    ; (this will prevent message signals when we're not waiting on messages)
    and  x0,a1                                  
    jclr #>EVENT_MESSAGE_BITPOS,a1,_EVENT_BUTTON
;    move #MenuQueueDescriptor,r0 ; Not needed, as the queue descriptor is passed in
    move y:MenuWaitingMsg,r4      ; Get the pointer to the message destination
    jsr GetMessage                            ;get the message
    move #>EVENT_MESSAGE,a                    ;set the flag
    jmp _end                                  ;
_EVENT_BUTTON
    jclr #EVENT_BUTTON_BITPOS,a1,_EVENT_TIMER ;if no button even, go check timer
    jsr SysGetButton                          ;get the button
    move y:<Const_ffffff,m0                   ;because some @#$^!% doesn't 
    ;get the pointer to the waiting message (its where we put the button event)
    move y:MenuWaitingMsg,r1 
    move r1,b                           ;see if the pointer to the msg is NULL
    tst b #>EVENT_BUTTON,a              ;set the return value to be EVENT_BUTTON
    jeq _error                          ;null pointer passed in for MSG
    move x0,y:(r1)                      ;copy the button event into the message
    jmp _end                            ;go to the end.
_EVENT_TIMER
    move #>EVENT_TIMER,a 
_end
    pop SSH
    nop
    rts
_error
    error


SysGetLastTime
;Returns last timeout value in a
    move y:MenuIntervalHigh,a
    move y:MenuIntervalLow,a0
    rts

SysSetTimer
;sets the timer of the menu module to the value in A
    move a1,y:MenuTimerHigh
    move a0,y:MenuTimerLow
    rts;

SysWaitOnEvent
    ; When called from 'C':  
    ; A1 is the mask, r0 is the pointer to the MSG, and B1 is the timer.
    bclr #EVENT_TIMER_BITPOS,a1   ; lets assume no timer
    move    a1,a
    tst     a
    jeq     _NoRepeat             ;no events listed, so don't repeat immediately
    ; We are asking for either a button or a message, so lets repeat the module
    bset    #EVENT_REPEAT_BITPOS,a1  
_NoRepeat
    move y:<Const_ffffff,y0	        ;move -1 into Y0
    cmp y0,b r0,y:MenuWaitingMsg    ;save off the pointer to the msg
    jeq _NO_EVENT_TIMER         ;if the timer is set to -1, don't set the timer!
    move b1,b
    tst b
    jne _TIMER_NOT_ZERO
    ; Makes sure that we get called back immediately if somebody enters '0' wait
    bset #EVENT_NONE_BITPOS,a1      
    jmp _NO_EVENT_TIMER
_TIMER_NOT_ZERO
    push a1                         ;save a1 since its our mask we're working with
    push b1                         ;saves off the timer increment
    jsr SysGetCurrentTime           ;returns time in A
    clr b					        ;clears B
    ; put the timeout/interval into B0 for easy addition (was passed in B1)
    pop b0                          
    ; Add the timeout/interval to the time value in A 
    ; (gotten from SysGetLastTime or SysGetTime)
    add b,a                         
    jsr SysSetTimer ;Sets the Menu Module's timer value to the one calculated above.
    clr  a
    pop a1                          ;pops out the event mask passed in A1
    bset #EVENT_TIMER_BITPOS,a1     ;set the event mask
_NO_EVENT_TIMER
    push SSH                        ;push who called us on the stack
    move #ReturnFromWait,r0         ;push the ReturnFromWait on the stack
    push r0                         ;
    jmp ContextSwitch               ;switch to the kernel

    endsec 

