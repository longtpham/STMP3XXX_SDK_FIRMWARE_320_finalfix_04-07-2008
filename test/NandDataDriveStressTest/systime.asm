;///////////////////////////////////////////////////////////////////////////////
;  Copyright(C) SigmaTel, Inc. 2000-2001
;
;  File        : systime.asm
;  Description : STMP System Time
;///////////////////////////////////////////////////////////////////////////////

    page    132,60,0,0,0
    opt     mex

    section SYSFUN_SysTime

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
    include "regsrtc.inc"
    include "regscore.inc"
    list

;///////////////////////////////////////////////////////////////////////////////
;   External Definitions
;///////////////////////////////////////////////////////////////////////////////

    ;Functions  
    global  SysCheckTime
    global  SysTime
    global  FSysTime
    global  SysTimeReset
    global  FSysTimeReset
    global  SysGetCurrentTestTime
    global  FSysGetCurrentTestTime
    global  SysDelay
    global  FSysDelay
    global  SysTimeIsr
    global  SysGetElapsedTime
    global  SysIsElapsedTime
    global  SysTimeout

;///////////////////////////////////////////////////////////////////////////////
;   External References
;///////////////////////////////////////////////////////////////////////////////

    nolist
    include "const.xref"
    list

;///////////////////////////////////////////////////////////////////////////////
;   Equates
;///////////////////////////////////////////////////////////////////////////////

;///////////////////////////////////////////////////////////////////////////////
;   P Memory
;///////////////////////////////////////////////////////////////////////////////

    org     p,"SYSFUN_SysTime_P":

;///////////////////////////////////////////////////////////////////////////////
;
;>  Name:           SysCheckTime
;
;   Type:           Function
;
;   Description:    Checks if specified time has passed.
;
;   Inputs:         
;                   x = Target time (ms)
;
;   Outputs:        
;                   x = Target time (ms)
;                   carry set = time has passed
;                   carry clear = time has not passed
;                   
;   Notes:          none
;<
;///////////////////////////////////////////////////////////////////////////////
SysCheckTime
    push    x1
    push    x0
    jsr     SysGetCurrentTestTime
    pop     x0
    pop     x1
    sub     x,a
    jge     _time_has_passed
    bclr    #HW_SR_C_BITPOS,sr
    rts
_time_has_passed
    bset    #HW_SR_C_BITPOS,sr
    rts

;///////////////////////////////////////////////////////////////////////////////
;
;>  Name:           SysTimeReset
;
;   Type:           Function
;
;   Description:    Sets system time to zero 
;
;   Inputs:         none
;
;   Outputs:        none
;                   
;   Notes:          
;<
;///////////////////////////////////////////////////////////////////////////////
SysTimeReset
FSysTimeReset

    clr     a
    move    a,x:HW_RTCUPR                ; clear the upper count
    move    a,x:HW_RTCLOWR               ; clear the lower count
    rts

    rts


;///////////////////////////////////////////////////////////////////////////////
;
;>  Name:           SysGetCurrentTestTime
;
;   Type:           Function
;
;   Description:    Get the current system time
;
;   Inputs:         none
;
;   Outputs:        a = current system time
;                   
;   Notes:          The system time returned is a count of milliseconds since
;                   the SysTimeInit function was called (which resets the time
;                   count).  SysTimeInit is typically called only at system
;                   startup.
;<
;///////////////////////////////////////////////////////////////////////////////
SysGetCurrentTestTime
FSysGetCurrentTestTime

    clr     a
    move    x:HW_RTCUPR,a1               ; get the upper count
    move    x:HW_RTCLOWR,a0              ; get the lower count
    rts

;///////////////////////////////////////////////////////////////////////////////
;
;>  Name:           SysGetElapsedTime
;
;   Type:           Function
;
;   Description:    Returns time elapsed
;
;   Inputs:         b = previous time
;
;   Outputs:        a = current time - previous time
;                   
;   Notes:          
;<
;///////////////////////////////////////////////////////////////////////////////
SysGetElapsedTime
    jsr     SysGetCurrentTestTime           ; function returns time in ms in a
    sub     b,a                         ; a=CurrentTime-PreviousTime in msec
    rts

;///////////////////////////////////////////////////////////////////////////////
;
;>  Name:           SysIsElapsedTime
;
;   Type:           Function
;
;   Description:    Check if ellapsed time from a specific time is greater 
;                   than a specific reference
;
;   Inputs:         b1:b0 is time origin in mSecs
;                   x1 is reference time in mSecs
;
;   Outputs:        sr.N = 0 if time elapsed
;                        = 1 if time not elapsed 
;                   
;   Notes:          
;<
;///////////////////////////////////////////////////////////////////////////////
SysIsElapsedTime
    jsr     SysGetElapsedTime
    move    a0,a
    cmp     x1,a
    rts
    
;///////////////////////////////////////////////////////////////////////////////
;
;>  Name:           SysDelay
;
;   Type:           Function
;
;   Description:    Delay a specified number of miliseconds
;
;   Inputs:         x0 : Number of milliseconds to delay
;
;   Outputs:        none
;                   
;   Notes:          This function completes synchronously.  Therefore, the
;                   thread that calls this function is delayed for the duration
;                   of the call.
;<
;///////////////////////////////////////////////////////////////////////////////
FSysDelay
    move    a,x0
SysDelay
    ; Add the milliseconds to the current time to get the timeout value
    jsr     SysGetCurrentTestTime
    move    y:<Const_000000,x1
    add     x,a
    move    a0,b0
    move    a1,b1
    move    a2,b2

_Wait
    ; Wait until the current time is greater than or equal to the timeout
    jsr     SysGetCurrentTestTime
    cmp     a,b
    jgt     _Wait

    rts


;///////////////////////////////////////////////////////////////////////////////
;>  Name:   SysTimeout
;
;   Type:           Function
;
;   Description:    Check if timeout has occurred and if need to calculate
;                   next timeout
;
;  Inputs:
;   R0 = Address of timeout variable (two words in y memory)
;   X0 = Duration in milliseconds (for calculating next timeout)
;
;  Outputs:
;    If timed out:     carry set   (and timeout variable updated)   
;    If not timed out: carry clear
;<
;///////////////////////////////////////////////////////////////////////////////

SysTimeout
    ; Check if the timeout has occurred
    clr     b
    move    y:(r0)+,b1
    move    y:(r0)-,b0
    jsr     SysGetCurrentTestTime
    cmp     a,b
    jle     _Timeout

    ; Clear the carry and exit
    bclr   #HW_SR_C_BITPOS,sr
    jmp    _Exit

_Timeout
    ; Calculate the next timeout value
    clr     b
    move    x0,b0
    add     a,b
    move    b1,y:(r0)+
    move    b0,y:(r0)-

    ; Set the carry
    bset   #HW_SR_C_BITPOS,sr

_Exit
    rts


    endsec

