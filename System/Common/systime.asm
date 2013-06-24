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
    ;include "sysequ.inc"	//stmp6550
    include "regscore.inc"
    include "regsrtc.inc"
    ;include "sysresources.inc"	//stmp6550
    ;include "msgequ.inc"	//stmp6550
 if (@DEF('FUNCLET'))           
    include "resource.inc"
 endif
 if (@def('USE_RTC_FOR_TIME'))
 else
 if (!@def('MTP_BUILD'))
 if (!@def('USBMSC_BUILD'))
     include "systimers.inc"
 endif
 endif
     include "regstimer.inc"
     include "regsicoll.inc" 
     include "irqequ.inc"	
     include "buttons.inc"
 endif
    list

;///////////////////////////////////////////////////////////////////////////////
;   External Definitions
;///////////////////////////////////////////////////////////////////////////////

    ;Functions  
    global  SysCheckTime
    global  SysTime
    global  FSysTime
    global  SysTimeInit
    global  SysGetCurrentTime
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
    if (@def('USE_RTC_FOR_TIME'))
    else
        include "systimers.xref"
    endif
    include "const.xref"
    list
    
 if (@DEF('FUNCLET'))           
    extern  SysCallFunclet
 endif

;///////////////////////////////////////////////////////////////////////////////
;   Equates
;///////////////////////////////////////////////////////////////////////////////
        
;///////////////////////////////////////////////////////////////////////////////
;   X Memory
;///////////////////////////////////////////////////////////////////////////////

    org     x,"SYSFUN_SysTime_X":

;///////////////////////////////////////////////////////////////////////////////
;   Y Memory
;///////////////////////////////////////////////////////////////////////////////

    org     y,"SYSFUN_SysTime_Y":
FSysTime
    if (@def('USE_RTC_FOR_TIME'))
    ; don't need anything for the 3410
    else ; if (@def('DEVICE_3410')||@def('DEVICE_3310'))
SysTime              dc      0
                     dc      0

; data for SysTimeIsr
Sti_RegSave_A2       dc      0
Sti_RegSave_A1       dc      0
Sti_RegSave_A0       dc      0
Sti_RegSave_R0       dc      0
Sti_RegSave_X0       dc      0
    endif ; if (@def('DEVICE_3410')||@def('DEVICE_3310'))

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
    jsr     SysGetCurrentTime
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
;>  Name:           SysTimeInit
;
;   Type:           Function
;
;   Description:    Sets system time to zero & starts the timer
;
;   Inputs:         none
;
;   Outputs:        none
;                   
;   Notes:          
;<
;///////////////////////////////////////////////////////////////////////////////
SysTimeInit

 if (@DEF('FUNCLET'))           

    ;If using funclet, function will be executed via funclet kernel
    CallFunclet RSRC_FUNCLET_SYSTIMEINIT

    org p,"SYSFUNCLET_SysTimeInit_P":
    
    dc      RSRC_FUNCLET_SYSTIMEINIT           ; Funclet Resource number

 endif

    if (@def('USE_RTC_FOR_TIME'))

        if (@def('DEVICE_3500'))
            ; The 3500 has a 64 pre-divider. To count 1 ms intervals
            ; with a 24Mhz Crystal, the divider must be set to 375
            ; Total Divider is 375*64 = 24000  
            move    #>375,x0
            move    x0,x:HW_RTC_DIVIDE
        endif

    else
    move    y:<Const_ffffff,m0
    move    m0,m1

    ; Genesis, baby
    clr     a
    move    a1,y:SysTime
    move    a0,y:SysTime+1

    ; Start the timer
    move    #>SYS_TIMER_NUM,x0
    move    #_TimerEnabled,r0
    jmp     SysEnableTimer
_TimerEnabled
    endif

    rts

 if (@DEF('FUNCLET'))

    ;If using funclet, switch counter back to original here
    org    p,"SYSFUN_SysTime_P":
    
 endif

;///////////////////////////////////////////////////////////////////////////////
;
;>  Name:           SysGetCurrentTime
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
FSysGetCurrentTime
SysGetCurrentTime

    if (@def('USE_RTC_FOR_TIME'))
    clr     a
    move    x:HW_RTCUP,a1               ; get the upper count
    move    x:HW_RTCLOW,a0              ; get the lower count
    rts
    else ; if (@def('DEVICE_3410')||@def('DEVICE_3310'))
    push     x0                         ; need to preserve x0

    ; Hardware Workaround -- Disable IVL at HW_IPR
    clr     a
    move    #>SYS_TIME_IVL_MASK,a1
    
    move    x:HW_IPR,x0
    push    x0
    and     x0,a
    move    a1,x:HW_IPR                    ; disables decoder timer IVL

    ; Get the time
    clr     a
    move    y:SysTime,a1
    move    y:SysTime+1,a0

    ; Hardware Workaround -- Restore IVL at HW_IPR
    pop     x0
    move    x0,x:HW_IPR                    ; re-enables the IVL line of decoder

    pop     x0                          ; restores x0
    rts
    endif ; if (@def('DEVICE_3410')||@def('DEVICE_3310'))

;///////////////////////////////////////////////////////////////////////////////
;
;>  Name:           SysGetElapsedTime
;
;   Type:           Function
;
;   Description:    Returns time elapsed
;
;   Inputs:         b = previous time for asm caller. Tasking C compiler puts DWORD in b1:b0 also.
;
;   Outputs:        a = current time - previous time
;                   
;   Notes:          
;<
;///////////////////////////////////////////////////////////////////////////////
FSysGetElapsedTime
SysGetElapsedTime
    jsr     SysGetCurrentTime           ; function returns time in ms in a
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
;   Inputs:         For asm callers: x0 : Number of milliseconds to delay
;                   For C   callers: a1 : Number of milliseconds to delay
;
;   Outputs:        none
;                   
;   Notes:          This function completes synchronously.  Therefore, the
;                   thread that calls this function is delayed for the duration
;                   of the call. Requesting 1ms may return in much less than 1ms. 
;<
;///////////////////////////////////////////////////////////////////////////////
FSysDelay
    move    a,x0
SysDelay
    ; Add the milliseconds to the current time to get the timeout value
    jsr     SysGetCurrentTime
    move    y:<Const_000000,x1
    add     x,a
    move    a0,b0
    move    a1,b1
    move    a2,b2

_Wait
    ; Wait until the current time is greater than or equal to the timeout
    jsr     SysGetCurrentTime
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
    jsr     SysGetCurrentTime
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


;///////////////////////////////////////////////////////////////////////////////
;
;>  Name:           SysTimeIsr
;
;   Type:           Interrupt Service Routine
;
;   Description:    Increments system time
;
;   Inputs:         none
;
;   Outputs:        !!!!!!!!!!! If this is modified & ends up using the hw stack then you must
;                   !!!!!!!!!!! save it first because this can interrupt the decoder and there's
;                   !!!!!!!!!!! no room left.  This isr must sit at the highest level so no other
;                   !!!!!!!!!!! interrupts can occur while it's running.
;                   
;   Notes:          ???
;<
;///////////////////////////////////////////////////////////////////////////////
SysTimeIsr
    if (@def('USE_RTC_FOR_TIME'))
    debug
    else ; if (@def('DEVICE_3410')||@def('DEVICE_3310'))

    ; Clear the DM bit (double-precision multiply mode, bit 6) of the Mode Register
    ; This will be returned to its previous state when the RTI occurs (since the SR
    ; will be popped back off System Stack Low (SSL).
    ; Accumulator behavior is unpredictable when the DM bit is set.
    ; Next cycle after clearing it should NOT be an ALU instruction.
    andi #$BF,mr

    move    a2,y:Sti_RegSave_A2
    move    a1,y:Sti_RegSave_A1
    move    a0,y:Sti_RegSave_A0
    move    r0,y:Sti_RegSave_R0
    move    x0,y:Sti_RegSave_X0
    clr     a
    move    y:SysTime,a1
    move    y:SysTime+1,a0
    inc     a
    move    a1,y:SysTime
    move    a0,y:SysTime+1

    ; Reset the timer
    move    #>SYS_TIMER_NUM,x0
    move    #_TimerReset,r0
    jmp     SysClearTimerIrq
_TimerReset

    move    y:Sti_RegSave_A2,a2
    move    y:Sti_RegSave_A1,a1
    move    y:Sti_RegSave_A0,a0
    move    y:Sti_RegSave_R0,r0
    move    y:Sti_RegSave_X0,x0
    rti
    endif ; if (@def('DEVICE_3410')||@def('DEVICE_3310'))
    endsec

