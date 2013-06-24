;///////////////////////////////////////////////////////////////////////////////
;  Copyright(C) SigmaTel, Inc. 2000-2003
;
;  File        : syspostmsg.asm
;  Description : Message posting routines
;///////////////////////////////////////////////////////////////////////////////

    section SYSFUN_SysPostMsg

;///////////////////////////////////////////////////////////////////////////////
;   Macros
;///////////////////////////////////////////////////////////////////////////////

    nolist
    include "sysmacro.asm"
    list

;///////////////////////////////////////////////////////////////////////////////
;   Includes
;///////////////////////////////////////////////////////////////////////////////

    nolist


    list
    
;///////////////////////////////////////////////////////////////////////////////
;   External Definitions
;///////////////////////////////////////////////////////////////////////////////

    ;Functions
    global  FSysPostMessage
    global  SysPostMessage
	global  Kernel_AddMessageToQueue

;///////////////////////////////////////////////////////////////////////////////
;   External References
;///////////////////////////////////////////////////////////////////////////////

    nolist

    include "const.xref"
    list

    extern     FKernel_SysPostMessage
    extern      QueueGetEmptySpace


;///////////////////////////////////////////////////////////////////////////////
;   Equates
;///////////////////////////////////////////////////////////////////////////////
    SysPostMessage: equ FKernel_SysPostMessage

;///////////////////////////////////////////////////////////////////////////////
;   P Memory
;///////////////////////////////////////////////////////////////////////////////

    org     p,".ptextkernel_syspostmessage":



;///////////////////////////////////////////////////////////////////////////////
;
;>  Name:           FSysPostMessage
;
;   Type:           Function
;
;   Description:    Post a message to module queue
;
;   Inputs:         R7 points to the end of the message
;
;   Outputs:        none
;                   
;   Notes:          C wrapper for SysPostMessage -- message passed on stack
;
;//egad...I hate to do it this way, but its the easiest way I can think of right now
;//Tasking pushes the parameters on the stack backwards, so we can't just offset the stack
;//by Length backwards and put this pointer in R0 to pass to SysPostMessage (note:  this C function 
;//is FSysPostMessage).  What ends up happening is we step backwards from the stack pointer, and push
;//these values onto the stack (making a message on the stack that is properly organized).  After calling
;//SysPostMessage, we fixup the stack properly to remove all the items we stuffed onto it.
;
;<
;///////////////////////////////////////////////////////////////////////////////
FSysPostMessage
    clr a   y:Const_ffffff,n7
    nop
    move y:(r7+n7),n1              ;get the length
    move r7,r1                ;set the pointer of r1 to the end.
    move    n1,a0
    lua (r1)-n1,r2               ;have r2 point to the beginning.
    asr a   r2,r0                ;we need to run trhough the loop for half of the length (or we'll double swap)
    jeq _enddo
    do a0,_enddo                ;take the message and flip it end to end.
        move y:(r2),x0
        move y:-(r1),x1
        move x1,y:(r2)+
        move x0,y:(r1)
_enddo
    push SSH                    
    jsr FKernel_SysPostMessage  ; now call the function to actually post the message
    pop SSH 
    nop
    rts


;///////////////////////////////////////////////////////////////////////////////
;
;>  Name:           Kernel_AddMessageToQueue
;
;   Type:           Function
;
;   Description:    Post a message to module queue
;
;   Inputs:         
;                   r0 : points to queue to put it in
;                   r4 : points to a message to be posted (in y memory)
;   Outputs:        none
;                   
;   Notes:          
;<
;///////////////////////////////////////////////////////////////////////////////
Kernel_AddMessageToQueue
    push    r0                      ;save off the queue pointer
    push    r4                      ;save off the message pointer
    jsr     QueueGetEmptySpace      ;get the empty space
    tfr     a,b                     ;move the empty space into b
    pop     r4                      
    pop     r0
    move    y:(r4),a                ; get the size of the message
    cmp     a,b                     ; compare size of message to post with space available in dest queue
    jlt     _exit
    tst     a                       ;make sure the length is at least 1
    ccerror eq                      ;if we're stopped here, then the message had a zero length!

_enough_space
    ; else continue processing posting message
    move    y:(r0)+,r1              ;get the head pointer
    move    y:(r0)-,m1              ;get the modulo value
    do      a1,_copy_loop
        move    y:(r4)+,a1
        move    a1,y:(r1)+
_copy_loop
    move    y:<Const_ffffff,m1
    clr     a    r1,y:(r0)                   ;set the return value to 'success' and update the head pointer
_rts
    rts
_exit
    jmp _rts

    endsec


    