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
    include "msgequ.inc"
    include "sysequ.inc"
    list
    
;///////////////////////////////////////////////////////////////////////////////
;   External Definitions
;///////////////////////////////////////////////////////////////////////////////

    ;Functions
    global  FSysPostMessage
    global  SysPostMessage

;///////////////////////////////////////////////////////////////////////////////
;   External References
;///////////////////////////////////////////////////////////////////////////////

    nolist
    include "bufdesc.xref"
    include "const.xref"
    list

    extern  SignalModule
    extern  ModuleRouterTable
    extern  ROUTER_TABLE_SIZE

;///////////////////////////////////////////////////////////////////////////////
;   Equates
;///////////////////////////////////////////////////////////////////////////////

;///////////////////////////////////////////////////////////////////////////////
;   X Memory
;///////////////////////////////////////////////////////////////////////////////
        
    org     x,"SYSFUN_SysPostMsg_X":

;///////////////////////////////////////////////////////////////////////////////
;  Y memory
;///////////////////////////////////////////////////////////////////////////////

    org     y,"SYSFUN_SysPostMsg_mod_Y":

;///////////////////////////////////////////////////////////////////////////////
;   P Memory
;///////////////////////////////////////////////////////////////////////////////

    org     p,"SYSFUN_SysPostMsg_P":



;///////////////////////////////////////////////////////////////////////////////
;
;>  Name:           FSysPostMessage
;
;   Type:           Function
;
;   Description:    Post a message to module queue
;
;   Inputs:         message to be posted
;
;   Outputs:        none
;                   
;   Notes:          C wrapper for SysPostMessage -- message past on stack
;
;//egad...I hate to do it this way, but its the easiest way I can think of right now
;//Tasking pushes the parameters on the stack backwards, so we can't just offset the stack
;//by Length backwards and put this pointer in R0 to pass to SysPostMessage (note:  this C function 
;//is FSysPostMessage).  What ends up happening is we step backwards from the stack pointer, and push
;//these values onto the stack (making a message on the stack that is properly organized).  After calling
;//SysPostMessage, we fixup the stack properly to remove all the items we stuffed onto it.
;//The warning "parameter iLength not used" is unavoidable without adding more code
;<
;///////////////////////////////////////////////////////////////////////////////
FSysPostMessage
    move r7,r1                  ;move our current stack pointer to r0 (this is where the message will start)
    move r7,r0
    move y:-(r1),a0              ;get the length, 
    move a0,y:(r7)+             ;put the length in the new message
    dec a
    do a0,_enddo                ;for each in the message
        move y:-(r1),x0         ;move backwards and copy it to x0
        move x0,y:(r7)+         ;push it on the stack, creating a new message
_enddo
    push a0                     ;Save off the length-1
    push SSH

    jsr SysPostMessage
    pop SSH
    pop n7                      ;pop the length-1 off into n7
    move (r7)-                  ;this is one for the length...
    move (r7)-n7                ;move the stack back by the length-1 (pop off the rest of the message)
    rts


;///////////////////////////////////////////////////////////////////////////////
;
;>  Name:           SysPostMessage
;
;   Type:           Function
;
;   Description:    Post a message to module queue
;
;   Inputs:         r0 : points to a message to be posted (in y memory)
;                   m0 : linear addressing
;
;   Outputs:        none
;                   
;   Notes:          
;<
;///////////////////////////////////////////////////////////////////////////////
SysPostMessage

    push    m1

    ;Find the buffer descriptor for the targetted module
    move y:(r0)+,y1                     ; y1=message size, loop value further down to copy the data
    move y:(r0)-,x1                     ; x1=message value
    
    ;Extract the base command from the message command
    ;and start init for the next loop
    move    #>$ff0000,a                 ; a1 = Mask (I do not care about a2)
    and     x1,a    #>ModuleRouterTable,r1

    ;r1 points to the Base Command entry for the first queue descriptor
    ;Buffer Descriptor Search Loop
    move    y:<Const_ffffff,m1          ;Continuing loop init ...
    move    #>$ff0000,b                 ; b1 = Mask (I do not care about b2, it should be the same than a2 (few lines above))
    
    do  #ROUTER_TABLE_SIZE,_EndLoop
        move    y:(r1)+,x0              ;x0 = CommandBase + Queue descriptor Address
        move    y:(r1)+,x1              ;x1 = Module Number
        and     x0,b                    ;Get rid of queue descriptor address
        cmp     a,b                     ;Check match
        jne     _Continue               ;No match, try next router entry
        enddo                           ;We have a match. Terminate the loop
        jmp     _FoundIt
_Continue
        move    #>$ff0000,b
_EndLoop

    error                               ;Does not know how to route message
    
_FoundIt
    move    x0,r4                       ;low 16 bits of x0 is the address of the queue descriptor

    ; Save the module number
    push x1

    ; Determine amount of empty space in that message queue.
    ; HACK need to save y1,r0,m0,r4,m4 on the stack when we get it working...
    ; ...they could be destroyed by the jsr below if GetEmptySpace is modified.
    jsr    GetEmptySpace

    ; compare size of message to post with space available in dest queue
    cmp    y1,a
    jge    _enough_space
    ; if not enough space, debug instruction
    error

_enough_space
    ; else continue processing posting message
    ; acquire the head pointer and module of the target queue
    move #2,n4                          ; set up offset reg for advancement
    move (r4)+                          ; skip past the base address
    move y:(r4)+n4,m1                   ; m1=target buffer queue modulo, r4 points to headptr
    move y:(r4),r1                      ; r1=head ptr of target buffer queue

    ; copy message type and arguments to destination queue...
    ; ...including the size field

    do   y1,_copy_message
        move y:(r0)+,x0
        move x0,y:(r1)+
_copy_message

    ; update the head pointer in the destination queue buffer descriptor
    move r1,y:(r4)
    ; restore the linear addressing mode
    move y:<Const_ffffff,m1

    ; Restore the module number
    pop  x1

    ; Signal the module that it has a message
    move #>EVENT_MESSAGE,x0
    jsr  SignalModule
    pop     m1

    rts

    endsec

