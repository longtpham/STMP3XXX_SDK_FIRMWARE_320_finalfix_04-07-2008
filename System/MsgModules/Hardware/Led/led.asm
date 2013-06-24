;///////////////////////////////////////////////////////////////////////////////
; Copyright(C) SigmaTel, Inc. 2000-2001
;
; Filename: led.asm
; Description: STMP LED Module
;///////////////////////////////////////////////////////////////////////////////

    opt     mex

    section LEDMOD_Led

;///////////////////////////////////////////////////////////////////////////////
;   Includes and external referencse
;///////////////////////////////////////////////////////////////////////////////
    nolist
    
    include "sysmacro.asm"
    include "stack.xref"
    include "const.xref"
    include "msgequ.inc"
    include "hwequ.inc"
    include "sysequ.inc"

    include "resource.inc"
    list

    page    132,60,0,0,0

    EXTERN      ModuleSetTimer
    extern      LedTable
    extern      LED_NUMBER
    extern      LED_TABLE_LENGTH
;///////////////////////////////////////////////////////////////////////////////
;   Symbol Exports 
;///////////////////////////////////////////////////////////////////////////////

;Functions
    global  LedProcessQueue
    global  LedInit
       
;Variable in P        
    global  ShortestTime
    global  FShortestTime
;///////////////////////////////////////////////////////////////////////////////
;   Equates
;///////////////////////////////////////////////////////////////////////////////


;///////////////////////////////////////////////////////////////////////////////
;  X memory
;///////////////////////////////////////////////////////////////////////////////


;///////////////////////////////////////////////////////////////////////////////
;  Y memory
;///////////////////////////////////////////////////////////////////////////////

     org   y,"LEDModule.y":



LedSP           ds      1

  
FShortestTime
ShortestTime    dc      -1           ; init the shortest time to $ffffff


;///////////////////////////////////////////////////////////////////////////////
;   Code
;///////////////////////////////////////////////////////////////////////////////

        org p,"LEDMOD_Led_overlay_p":

        ; Resource header
    dc      RSRC_LEDMOD_CODE         ; Resource number




;///////////////////////////////////////////////////////////////////////////////
;> Name: LedInit
;
;  Type: Function
;  Description: Initialize the LED Module
;  Inputs: 
;  Outputs: 
;  Notes: 
;<
;///////////////////////////////////////////////////////////////////////////////
LedInit

    move    y:<Const_000000,x0  
    move    y:<Const_ffffff,m0
    move    y:<Const_ffffff,m4

    do    #LED_NUMBER,_done_init
        ; calculate the offset to the correct LED structure
        move  #>LED_TABLE_LENGTH,y1       
        mpy   y1,x0,b             
        asr   b

        move  b0,y0         ; set up b accumulator for the add
        move  y0,b1

        ; set up a pointer to the correct LED structure
        move  #LedTable,y0
        add   y0,b
        move  b1,r0             

        ; Enable the bits in the GPIO Enable reg
        move    #3,n0
        nop
        move    y:(r0+n0),x1                          ; LED On BIT MASK
        move    y:(r0),r4
        nop

        ; Enable the GPIO bits
        move    x:(r4),a
        or      x1,a                                
        move    a,x:(r4)                

        ; Enable the GPIO as output
        move    #3,n4
        nop
        move    x:(r4+n4),a
        or      x1,a                                
        move    a,x:(r4+n4)                

        ; Reset the enabled bits as zero's
        move    #4,n0 
        move    #1,n4
        move    y:(r0+n0),x1  
        move    x:(r4+n4),a                     ; LED On BIT MASK  
        and     x1,a                         ; current data in reg
        move    a,x:(r4+n4)
    
        ; increment the led number
        move    x0,a0
        inc     a
        move    a0,x0
_done_init
    rts
       


;///////////////////////////////////////////////////////////////////////////////
;> Name: LedProcessQueue
;
;  Type: Function
;  Description: Process the LED Module message queue.  This is the entry point
;               from the Executive.
;  Inputs:
;       r0=Descriptor for this modules message queue (in y memory)
;       m0=linear
;  Outputs: 
;  Notes: 
;<
;///////////////////////////////////////////////////////////////////////////////
LedProcessQueue
    push    m1

    ; Save the system stack
    move    #_StackSaved,r6              ; setup the return address
    move    #LedSP,r5                    ; pointer to the SP local save
    jmp     SaveSystemStack              ; save the hardware stack
_StackSaved

    ; Put the queue modulo into m1
    move    #>BD_BufferModulo,n0
    nop
    move    y:(r0+n0),m1

    ; Put the head pointer into a
    move    #BD_HdPtr,n0
    nop
    move    y:(r0+n0),a

    ; Put the tail pointer into b
    move    #BD_TailPtr,n0
    nop
    move    y:(r0+n0),b

    ; Check if the queue is empty
    jmp     _CheckDone

_ProcessMessage

    ; Put the tail pointer into r1
    move    b1,r1
    
    ; Save working variables
    push    a1                      ; Head pointer
    push    r0                      ; Pointer to queue descriptor
    push    r1                      ; Tail pointer
    push    m1                      ; Queue modulo

    ; Put the message ID into b
    move    #>MSG_ID,n1
    nop
    move    y:(r1+n1),b

    ; Verify that the message ID is valid by checking message range
    move    #>LED_FIRST_MSG_ID,x0
    cmp     x0,b
    jlt     _InvalidMessageID
    move    #>LED_LAST_MSG_ID,x0
    cmp     x0,b
    jgt     _InvalidMessageID

    ; Point r1 to the first argument of the message
    move    (r1)+
    move    (r1)+

    ; Clear the upper byte of the message ID
    move    #$00FFFF,x0
    and     x0,b

    ; Jump to the message handler
    move    #_JumpTable,r2
    move    y:<Const_ffffff,m2
    move    b1,n2
    nop
    movem   p:(r2+n2),r3
    nop
    jmp     (r3)
    
    
_LedControl
    ; Get the arguments
    move    y:(r1)+,x0              ; LED number (used to be a1)
    move    y:(r1)+,a1              ; ON time
    move    y:(r1)+,a0              ; OFF time
    move    y:<Const_000000,x1      ; delay on time

    jsr     LedControl

    jmp     _NextMessage

_LedDlyOnControl
    ; Get the arguments
    move    y:(r1)+,x0              ; LED number (used to be a1)
    move    y:(r1)+,a1              ; ON time
    move    y:(r1)+,a0              ; OFF time
    move    y:(r1)+,x1              ; Delayed on time
    jsr     LedControl

    jmp     _NextMessage

_LedReset
    ; Get the arguments
    move    y:(r1)+,x0              ; LED number

    jsr     LedReset

    jmp     _NextMessage

_InvalidMessageID

    error

_NextMessage
    ; Restore working variables
    pop     m1                      ; Queue modulo
    pop     r1                      ; Tail pointer
    pop     r0                      ; Pointer to buffer descriptor
    pop     a                       ; Head pointer

    ; Add the message length to the tail pointer
    move    y:(r1),n1
    nop
    move    (r1)+n1
    nop
    move    r1,b

_CheckDone


    ; Check if head pointer equals the tail pointer
    cmp     a,b
    jne     _ProcessMessage

    ; Update the buffer descriptor with the new tail pointer
    move    #>BD_TailPtr,n0
    nop
    move    b1,y:(r0+n0)

    ; Restore the system stack
    move    #LedSP,r5                    ; pointer to the SP local save
    move    #_Leave,r6                   ; setup the return address
    jmp     RestoreSystemStack           ; restore the hardware stack
_Leave


;;;;;;;;;;;;;;;;  blinking code  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;
    
    
    ; Search for zeros in the current time variable
    move    #LedTable,r4
    move    y:<Const_ffffff,m4
    move    y:<Const_ffffff,m5
     
    
    do      #LED_NUMBER,_end_scan1
        ; Check to see if Current ON TIME = 0
        move  #7,n4
        nop
        move  y:(r4+n4),a             ; move current on time into A
        move  y:<Const_000000,x1  
        cmp   x1,a
        jne   _check_off_time
          
         ; Turn the LED off
        move  #2,n4
        nop
        move  y:(r4+n4),x1          ; get the dataout register
        move  x1,r5
        nop
        move  x:(r5),x1                  ; get contents of dataout reg
        move  #4,n4
        nop
        move  y:(r4+n4),a1          ; get off mask
        and    x1,a1                  ; and the LED bit with GPIO data reg
        move  a1,x:(r5)               ; put the register back

        ; Copy the original on+off time.
        
        ; First Check if on or off times = 0
        move  #5,n4
        nop
        move  y:(r4+n4),a          ; move off time into A
        move  y:<Const_000000,x1  
        cmp   x1,a
        jeq   _end

        move  #6,n4
        nop
        move  y:(r4+n4),a          ; move off time into A
        move  y:<Const_000000,x1  
        cmp   x1,a
        jeq   _end
         
        ; proceed to add together and copy

        move  #5,n4
        nop
        move  y:(r4+n4),a
        move  #6,n4
        nop
        move  y:(r4+n4),b
        add   a,b
        move  #7,n4
        nop
        move  b1,y:(r4+n4)

        jmp   _end

_check_off_time

        ; Check to see if OFF TIME = 0
        move  #8,n4
        nop
        move  y:(r4+n4),a          ; move off time into A
        move  y:<Const_000000,x1  
        cmp   x1,a
        jne   _end

        ; Turn the LED on
        move  #2,n4
        nop
        move  y:(r4+n4),x1          ; get the dataout register
        nop
        move  x1,r5
        nop
        move  x:(r5),x1                  ; get contents of dataout reg
        move  #3,n4
        nop
        move  y:(r4+n4),a1          ; get on mask
        or    x1,a1              ; or the LED bit with GPIO data reg
        move  a1,x:(r5)               ; put the register back
    
        ; Copy the original off time.
        ; First Check if on or off times = 0
        move  #5,n4
        nop
        move  y:(r4+n4),a          ; move off time into A
        move  y:<Const_000000,x1  
        cmp   x1,a
        jeq   _end
    
        move  #6,n4
        nop
        move  y:(r4+n4),a          ; move off time into A
        move  y:<Const_000000,x1  
        cmp   x1,a
        jeq   _end
    
        ; proceed to add together and copy
        move  #5,n4
        nop
        move  y:(r4+n4),a
        move  #6,n4
        nop
        move  y:(r4+n4),b
        add   a,b
        move  #8,n4
        nop
        move  b1,y:(r4+n4) 
    
        jmp   _end


_end
        rep   #LED_TABLE_LENGTH
            move  (r4)+
        nop
        nop

_end_scan1

        ; Find shortest current time which does not equal to 0.
    move    #LedTable,r4 
    move    y:<Const_ffffff,y0
    move    y0,y:ShortestTime

    do      #LED_NUMBER,_end_scan2
        move  #8,n4
        nop
        move  y:(r4+n4),y0          ; set pointer to current off time
        move  y:<Const_000000,a
        cmp   y0,a
        jeq   _do_not_update          ; check if it's zero

        move  #7,n4
        nop
        move  y:(r4+n4),y0          ; set pointer to current on time
        move  y:<Const_000000,a
        cmp   y0,a
        jeq   _do_not_update          ; check if it's zero
        clr   a
        move  y:ShortestTime,a1
        cmp   y0,a
        jmi   _do_nothing_w_current_on_time ; check if the current on time is 
                            ; less then the shortest time

                         ; save the current on time as the 
        move  y0,y:ShortestTime      ; shortest time
_equal_to_zero1          
_do_nothing_w_current_on_time
        
        move  #8,n4
        nop
        move  y:(r4+n4),y0         ; set pointer to current off time
        clr   a
        move  y:ShortestTime,a1
        cmp   y0,a
        jmi   _do_nothing_w_current_off_time ; check if the current off time is 
                         ; is less then the shortest time
        move  y0,a             ; save the current off time as the 
        move  a,y:ShortestTime       ; shortest time

_equal_to_zero2          
_do_nothing_w_current_off_time
_do_not_update      
        rep   #LED_TABLE_LENGTH
            move  (r4)+
        nop
        nop


_end_scan2
     
            ; subtract shortest current time from current times
    move    #LedTable,r4 
    
    do      #LED_NUMBER,_end_scan3
        move  #8,n4             ; set pointer to current on time
        nop
        move  y:(r4+n4),y0
        move  y:<Const_000000,a
        cmp   y0,a
        jeq   _equal_to_zero4         ; check if it's zero
    
        move  #7,n4             ; set pointer to current on time
        nop
        move  y:(r4+n4),a
        move  y:<Const_000000,y0
        cmp   y0,a
        jeq   _equal_to_zero4         ; check if it's zero
    
    
        move  y:ShortestTime,y1
        sub      y1,a
        move  a1,y:(r4+n4)         
_equal_to_zero3
        move  #8,n4             ; set pointer to current on time
        nop
        move  y:(r4+n4),a

        move  y:ShortestTime,y1
        sub      y1,a
        move  a1,y:(r4+n4)    
_equal_to_zero4
        rep   #LED_TABLE_LENGTH
            move  (r4)+
        nop
        nop

_end_scan3
    ; Check to see if ShortestTime is equal to $ffffff
    move  y:ShortestTime,x0      ; set the time in milliseconds   
    move  y:<Const_ffffff,a
    cmp   x0,a
    jeq   _skip_timer
    ; set timer to shortest temp time
    move  #>EVENT_MESSAGE,a1   ; I want to be called again when I have a message
    jsr   ModuleSetTimer
    jmp   _done

_skip_timer
    move  #>EVENT_MESSAGE,a1   ; I want to be called again when I have a message
    
_done    

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;


        pop     m1

        rts

_JumpTable
        dc      _LedControl
        dc      _LedReset
        dc      _LedDlyOnControl

; Note: It is important that the entries in the jump table be in the
; same order as the LED message ID's which are defined in msgequ.inc




;///////////////////////////////////////////////////////////////////////////////
;> Name: LedControl
;
;  Type: Function
;  Description: Controls the ON and OFF times of the LEDs
;  Inputs:
;       x0 = LED #
;       a1 = ON time
;       a0 = OFF time
;	x1 = delayed on Time
;  Outputs: 
;  Notes: 
;<
;///////////////////////////////////////////////////////////////////////////////
LedControl

    ; calculate the offset to the correct LED structure
    move  #>LED_TABLE_LENGTH,y1       
    mpy   y1,x0,b             
    asr   b
    
    move  b0,y0                     ; set up b accumulator for the add
    move  y0,b1
    
    ; set up a pointer to the correct LED structure
    move  #LedTable,y0
    add   y0,b
    move  b1,r0             

    ; Store the arguments in the LED structure
    move  #5,n0
    nop
    move  a1,y:(r0+n0)                 ; save on time  in milliseconds
    move  #7,n0
    nop
    move  a1,y:(r0+n0)               ; save on time in current time
    nop
    move  #6,n0
    nop
    move  a0,y:(r0+n0)                 ; save off time in milliseconds
    move  #8,n0
    nop
    move  a0,y:(r0+n0)                 ; save off time in current time
    ; test to see if the delayed on time is specified, i.e. not zero
    nop
    clr   a
    cmp   x1,a
    jeq   _no_Delay_on
    move  x1,y:(r0+n0)                 ; put the delayed on time in current time
    nop
    clr	b
    move  y:(r0+n0),b
    move  #7,n0
    nop
    move  y:(r0+n0),a                 ; save on time  in milliseconds
    ; Add the on time to delay time
    add   a,b
    nop
    move  b1,y:(r0+n0)                 ; store ontime + delay time in current time
    jmp	  _dont_add
_no_Delay_on
    ;=======================================================-=
    ; If neither on nor off time is equal to zero then add them
    ; together and store in off time.

    ; Check to see if ON TIME = 0
    move  #5,n0
    nop
    clr   a
    move  y:(r0+n0),a                 ; move on time into A
    move  y:<Const_000000,x1  
    cmp   x1,a
    jeq   _dont_add
    ; Check to see if OFF TIME = 0
    move  #6,n0
    nop
    clr   b
    move  y:(r0+n0),b                 ; move off time into A
    move  y:<Const_000000,x1  
    cmp   x1,b
    jeq   _dont_add

    ; Add the off time to On time
    add   a,b
    move  #8,n0
    nop
    move  b1,y:(r0+n0)                 ; save off time in current time

_dont_add
    ;=========================================================

    ; Check to see if ON TIME = 0
    move  #7,n0
    nop
    move  y:(r0+n0),a                 ; move on time into A
    move  y:<Const_000000,x1  
    cmp   x1,a
    jne   _check_off_time

    ; Turn the LED off
    move  #2,n0
    nop
    move  y:(r0+n0),x1
    nop
    move  x1,r4
    nop
    move  x:(r4),x1         
    move  #4,n0
    nop
    move  y:(r0+n0),a1
    and   x1,a1                     ; and the LED bit with GPIO data reg
    nop
    move  a1,x:(r4)                  ; put the register back

    jmp   _end

_check_off_time

    ; Check to see if OFF TIME = 0
    move  #6,n0
    nop
    move  y:(r0+n0),a                 ; move off time into A
    move  y:<Const_000000,x1  
    cmp   x1,a
    jne   _end

    ; Turn the LED on
    move  #2,n0
    nop
    move  y:(r0+n0),x1
    nop
    move  x1,r4
    nop
    move  x:(r4),x1           
    move  #3,n0
    nop
    move  y:(r0+n0),a1
    or    x1,a1                     ; or the LED bit with GPIO data reg
    nop
    move  a1,x:(r4)                  ; put the register back
    jmp   _end

_end

    rts




;///////////////////////////////////////////////////////////////////////////////
;> Name: LedReset
;
;  Type: Function
;  Description: Forces the LED to the default state
;  Inputs: 
;       x0 = LED #
;  Outputs: 
;  Notes: 
;<
;///////////////////////////////////////////////////////////////////////////////
LedReset


    ; calculate the offset to the correct LED structure
    move  #>LED_TABLE_LENGTH,y1       
    mpy   y1,x0,b             
    asr   b
    
    move  b0,y0                      ; set up b accumulator for the add
    move  y0,b1
    
    ; set up a pointer to the correct LED structure
    move  #LedTable,y0
    add   y0,b
    move  b1,r0             

    ; Store 0s in the LED structure

    clr   a
    move  #5,n0
    nop
    move  a1,y:(r0+n0)                  ; save on time  in milliseconds
    move  #7,n0
    nop
    move  a1,y:(r0+n0)                ; save on time in current time
    nop
    move  #6,n0
    nop
    move  a0,y:(r0+n0)                  ; save off time in milliseconds
    move  #8,n0
    nop
    move  a0,y:(r0+n0)                  ; save off time in current time
    
    rts
                       
    endsec






